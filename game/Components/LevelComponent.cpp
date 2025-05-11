#include "LevelComponent.h"

#include "Animator.h"
#include "DamageCommand.h"
#include "HealthComponent.h"
#include "ImageRendererComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LevelLoader.h"
#include "MoveCommands.h"
#include "MovementComponent.h"
#include "ResourceManager.h"
#include "ScoreCommand.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::LevelComponent::LevelComponent(kob::GameObject& parent, const std::filesystem::path& levelPath)
	: Component(parent)
{
	m_vTiles = LevelLoader::LoadCSV(levelPath, m_Rows, m_Cols);

	for (uint32_t i{}; i < m_vTiles.size(); ++i)
	{
		if (m_vTiles[i] == TileType::SpawnChef)
		{
			m_ChefSpawn = i;
			break;
		}
	}
}

void bt::LevelComponent::SpawnTileMap(float tileSize)
{
	m_TileSize = tileSize;
	for (uint32_t y{}; y < m_Rows; ++y)
	{
		for (uint32_t x{}; x < m_Cols; ++x)
		{
			std::string spritePath{};
			switch (m_vTiles[y * m_Cols + x])
			{
			case TileType::Platform:
			{
				spritePath = GetPlatformPath(x);
				AddTileGameObject(spritePath, { x,y }, { tileSize / 2, tileSize / 2 });
				break;
			}
			case TileType::Plate:
			{
				const float pieceSize = tileSize / 2;
				const float pieceOffset = tileSize / 4;
				for (int i{}; i < 2; ++i)
				{
					float offsetX = static_cast<float>(i % 2) * pieceSize;
					float offsetY = 16.f;

					int subType = GetTileSubType(x, y, TileType::Plate);
					if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
						spritePath = "level/tiles/PlateL.png";
					else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
						spritePath = "level/tiles/PlateR.png";
					else
						spritePath = "level/tiles/PlateM.png";

					AddTileGameObject(spritePath, { x,y }, { offsetX + pieceOffset, offsetY + pieceOffset });
				}
				break;
			}
			case TileType::Ladder:
			{
				spritePath = "level/tiles/Ladder.png";
				AddTileGameObject(spritePath, { x,y }, { tileSize / 2, tileSize / 2 });
				break;
			}
			case TileType::LadderPlatform:
			{
				spritePath = GetLadderPlatformPath(x);
				AddTileGameObject(spritePath, { x,y }, { tileSize / 2, tileSize / 2 });
				break;
			}
			case TileType::BottomBun:
			{
				AddIngredientTile(TileType::BottomBun, "level/tiles/BottomBun", x, y);
				break;
			}
			case TileType::TopBun:
			{
				AddIngredientTile(TileType::TopBun, "level/tiles/TopBun", x, y);
				break;
			}
			case TileType::Lettuce:
			{
				AddIngredientTile(TileType::Lettuce, "level/tiles/Lettuce", x, y);
				break;
			}
			case TileType::Burger:
			{
				AddIngredientTile(TileType::Burger, "level/tiles/Burger", x, y);
				break;
			}
			case TileType::Tomato:
			{
				AddIngredientTile(TileType::Tomato, "level/tiles/Tomato", x, y);
				break;
			}
			case TileType::Cheese:
			{
				AddIngredientTile(TileType::Cheese, "level/tiles/Cheese", x, y);
				break;
			}
			case TileType::Empty:
			{
				break;
			}
			default:
			{
				if (IsLadderAbove(x, y))
				{
					spritePath = GetLadderPlatformPath(x);
					AddTileGameObject(spritePath, { x,y }, { tileSize / 2, tileSize / 2 });
				}
				else
				{
					spritePath = GetPlatformPath(x);
					AddTileGameObject(spritePath, { x,y }, { tileSize / 2, tileSize / 2 });
				}
				break;
			}
			}
		}
	}
	SpawnChef();
}


//--------------------------------------------------
//    Helpers
//--------------------------------------------------
uint32_t bt::LevelComponent::PosToIdx(const glm::vec2& pos) const
{
	auto colRow = PosToColRow(pos);
	return colRow.y * m_Cols + colRow.x;
}
uint32_t bt::LevelComponent::PosToIdx(const glm::vec3& pos) const
{
	return PosToIdx({pos.x, pos.y});
}
glm::uvec2 bt::LevelComponent::PosToColRow(const glm::vec2& pos) const
{
	return {
		static_cast<int>(pos.x / m_TileSize),
		static_cast<int>(pos.y / m_TileSize)
	};
}
glm::uvec2 bt::LevelComponent::PosToColRow(const glm::vec3& pos) const
{
	return PosToColRow({pos.x, pos.y});
}
glm::vec2 bt::LevelComponent::IdxToCenterPos(uint32_t idx) const
{
	const int x = idx % m_Cols;
	const int y = idx / m_Cols;
	return { x * m_TileSize + m_TileSize / 2.f, y * m_TileSize + m_TileSize / 2.f };
}
glm::vec2 bt::LevelComponent::ColRowToCenterPos(const glm::uvec2& colRow) const
{
	return IdxToCenterPos(colRow.y * m_Cols + colRow.x);
}

bool bt::LevelComponent::IsIngredientTile(TileType tile) const
{
	return tile == TileType::BottomBun
		|| tile == TileType::TopBun
		|| tile == TileType::Burger
		|| tile == TileType::Lettuce
		|| tile == TileType::Tomato
		|| tile == TileType::Cheese;
}

bool bt::LevelComponent::IsAlignedVertically(const glm::vec2& pos, float threshold) const
{
	const auto i = PosToIdx(pos);
	const auto p = IdxToCenterPos(i);
	return abs(p.y - pos.y) < threshold;
}
bool bt::LevelComponent::IsAlignedHorizontally(const glm::vec2& pos, float threshold) const
{
	const auto i = PosToIdx(pos);
	const auto p = IdxToCenterPos(i);
	return abs(p.x - pos.x) < threshold;
}

bool bt::LevelComponent::CanMoveTo(uint32_t col, uint32_t row) const
{
	if (col >= m_Cols || row >= m_Rows)
		return false;

	auto tile = m_vTiles[col + row * m_Cols];
	return tile == TileType::Ladder
		|| tile == TileType::LadderPlatform
		//|| tile == TileType::HiddenLadder

		|| tile == TileType::Platform
		//|| tile == TileType::Plate
		//|| tile == TileType::Empty

		|| tile == TileType::TopBun
		|| tile == TileType::Tomato
		|| tile == TileType::Lettuce
		|| tile == TileType::Cheese
		|| tile == TileType::Burger
		|| tile == TileType::BottomBun

		|| tile == TileType::SpawnChef
		|| tile == TileType::SpawnEgg
		|| tile == TileType::SpawnHotDog
		|| tile == TileType::SpawnPickle
	;
}


//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
glm::vec2 bt::LevelComponent::GetChefSpawn()											const	{ return IdxToCenterPos(m_ChefSpawn); }
bt::LevelComponent::TileType bt::LevelComponent::GetTileType(uint32_t idx)				const
{
	if (idx >= m_vTiles.size())
		return TileType::Empty;
	return m_vTiles[idx];
}
bt::LevelComponent::TileType bt::LevelComponent::GetTileType(const glm::uvec2& colRow)	const
{
	return GetTileType(colRow.y * m_Cols + colRow.x);
}

glm::uvec2 bt::LevelComponent::GetColRow() const { return {m_Cols, m_Rows}; }
float bt::LevelComponent::GetTileSize() const { return m_TileSize; }


//--------------------------------------------------
//    Information
//--------------------------------------------------
int bt::LevelComponent::GetTileSubType(uint32_t x, uint32_t y, TileType type) const
{
	uint32_t leftCol = x - 1;
	uint32_t rightCol = x + 1;

	if (leftCol == 0xFFFFFFFF)
		return -1;
	if (rightCol >= m_Cols)
		return 1;

	const uint32_t neighbourIdxL = y * m_Cols + leftCol;
	const uint32_t neighbourIdxR = y * m_Cols + rightCol;

	if (m_vTiles[neighbourIdxL] == type &&
		m_vTiles[neighbourIdxR] == type)
		return 0;

	if (m_vTiles[neighbourIdxL] == type
		&& m_vTiles[neighbourIdxR] != type)
		return 1;

	if (m_vTiles[neighbourIdxL] != type
		&& m_vTiles[neighbourIdxR] == type)
		return -1;

	return 0;
}
bool bt::LevelComponent::IsLadderAbove(uint32_t x, uint32_t y) const
{
	uint32_t upRow = y - 1;
	if (upRow >= m_Rows)
		return false;

	uint32_t neighbourIdxU = upRow * m_Cols + x;
	if (m_vTiles[neighbourIdxU] == TileType::Ladder || m_vTiles[neighbourIdxU] == TileType::LadderPlatform)
		return true;
	return false;
}

std::string bt::LevelComponent::GetPlatformPath(uint32_t x) const
{
	if (x % 4 == 0)
		return "level/tiles/PlatformLight.png";
	if (x % 2 == 0)
		return "level/tiles/PlatformDark2.png";
	return "level/tiles/PlatformDark.png";
}
std::string bt::LevelComponent::GetLadderPlatformPath(uint32_t x) const
{
	if (x % 4 == 0)
		return "level/tiles/LadderPlatformLight.png";
	return "level/tiles/LadderPlatformDark.png";
}

void bt::LevelComponent::AddTileGameObject(const std::string& texturePath, const glm::uvec2& xy, const glm::vec2& offset) const
{
	auto& scene = GetParent()->GetScene();
	auto& tileObj = scene.AddEmpty();
	tileObj.SetParent(GetParent());
	tileObj.SetLocalPosition({ xy.x * m_TileSize + offset.x, xy.y * m_TileSize + offset.y, 0 });
	tileObj.SetLocalScale({ 2, 2, 2 });
	if (!texturePath.empty())
		tileObj.AddComponent<kob::ImageRendererComponent>(texturePath);
}
void bt::LevelComponent::AddIngredientTile(TileType type, const std::string& basePath, uint32_t x, uint32_t y) const
{
	std::string spritePath{};
	if (IsLadderAbove(x, y))
	{
		spritePath = GetLadderPlatformPath(x);
		AddTileGameObject(spritePath, { x,y }, { m_TileSize / 2, m_TileSize / 2 });
	}
	else
	{
		spritePath = GetPlatformPath(x);
		AddTileGameObject(spritePath, { x,y }, { m_TileSize / 2, m_TileSize / 2 });
	}

	const float pieceSize = m_TileSize / 2;
	const float pieceOffset = m_TileSize / 4;
	for (int i{}; i < 2; ++i)
	{
		float offsetX = static_cast<float>(i % 2) * pieceSize;
		float offsetY = pieceSize;

		int subType = GetTileSubType(x, y, type);
		if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
			spritePath = basePath + "L.png";
		else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
			spritePath = basePath + "R.png";
		else if (abs(offsetX) >= FLT_EPSILON)
			spritePath = basePath + "M1.png";
		else
			spritePath = basePath + "M2.png";

		AddTileGameObject(spritePath, { x,y }, { offsetX + pieceOffset, offsetY + pieceOffset });
	}
}
void bt::LevelComponent::SpawnChef() const
{
	constexpr float chefWalkDelay = 0.1f;
	constexpr int chefTxtSize = 16;
	constexpr float speed = 50.f;
	auto chefSheet = kob::ResourceManager::GetInstance().LoadSpriteSheet("characters/ChefSheet.png",
	        {
		        {"Down", {
			        {
			            { 0, 16, chefTxtSize, chefTxtSize},
			            {16, 16, chefTxtSize, chefTxtSize},
			            {32, 16, chefTxtSize, chefTxtSize},
			            {16, 16, chefTxtSize, chefTxtSize},
			        }, chefWalkDelay} },
		        {"Up", {
			        {
			            { 96, 16, chefTxtSize, chefTxtSize},
			            {112, 16, chefTxtSize, chefTxtSize},
			            {128, 16, chefTxtSize, chefTxtSize},
			            {112, 16, chefTxtSize, chefTxtSize},
			        }, chefWalkDelay} },
		        {"Left", {
			        {
			            {48, 16, chefTxtSize, chefTxtSize},
			            {64, 16, chefTxtSize, chefTxtSize},
			            {80, 16, chefTxtSize, chefTxtSize},
			            {64, 16, chefTxtSize, chefTxtSize},
			        }, chefWalkDelay} },
		        {"Right", {
			        {
			            {48, 0, chefTxtSize, chefTxtSize},
			            {64, 0, chefTxtSize, chefTxtSize},
			            {80, 0, chefTxtSize, chefTxtSize},
			            {64, 0, chefTxtSize, chefTxtSize},
			        }, chefWalkDelay} }
	        });
	
	auto& scene = GetParent()->GetScene();
	auto& chef = scene.AddEmpty("Player1");
	chef.SetParent(GetParent());
	const auto chefHealth = chef.AddComponent<HealthComponent>(4);
	chefHealth->OnDamageTaken() += [] { kob::ServiceLocator<kob::ISoundSystem>::GetService().Pause("sound/BGM.wav");
		kob::ServiceLocator<kob::ISoundSystem>::GetService().Play("sound/Death.wav", 0.25f, 0); };
	auto chefScore = chef.AddComponent<ScoreComponent>();
	const auto renderComp = chef.AddComponent<kob::ImageRendererComponent>(chefSheet->GetTexture());
	const auto animator = chef.AddComponent<kob::Animator>(renderComp, chefSheet);
	const auto chefMovement = chef.AddComponent<MovementComponent>(speed);
	chefMovement->SetCurrentLevel(*this);
	animator->Play("Down", false);
	auto chefSpawn = GetChefSpawn();
	chef.SetLocalPosition({ chefSpawn.x, chefSpawn.y, 0 });
	chef.SetLocalScale(glm::vec3(2, 2, 2));

	auto& inputManager = kob::InputManager::GetInstance();
	inputManager.RegisterKeyboardCmd(SDLK_w, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0, -1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_s, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0,  1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_d, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 1,  0, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_a, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ -1,  0, 0 }));

	inputManager.RegisterKeyboardCmd(SDLK_c, kob::TriggerState::Pressed, std::make_unique<DamageCommand>(*chefHealth));
	inputManager.RegisterKeyboardCmd(SDLK_z, kob::TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 10));
	inputManager.RegisterKeyboardCmd(SDLK_x, kob::TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 100));
}
