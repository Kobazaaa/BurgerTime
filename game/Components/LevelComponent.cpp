#include "LevelComponent.h"
#include "Animator.h"
#include "DamageCommand.h"
#include "HealthComponent.h"
#include "ImageRendererComponent.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LevelLoader.h"
#include "PlayingCommands.h"
#include "MovementComponent.h"
#include "ColliderComponent.h"
#include "EnemyAILogicComponent.h"
#include "IngredientTileComponent.h"
#include "IngredientComponent.h"
#include "ResourceManager.h"
#include "ScoreCommand.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"
#include "SquashableComponent.h"
#include "StunnableComponent.h"

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
			switch (m_vTiles[y * m_Cols + x])
			{
			case TileType::Platform:
			{
				AddPlatformTile(GetPlatformPath(x), { x,y }, true);
				break;
			}
			case TileType::Plate:
			{
				const float pieceSize = tileSize / 2;
				const float pieceOffset = tileSize / 4;
				for (int i{}; i < 2; ++i)
				{
					const float offsetX = static_cast<float>(i % 2) * pieceSize;
					const float offsetY = 16.f;
					std::string spritePath{};

					int subType = GetTileSubType(x, y, TileType::Plate);
					if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
						spritePath = "level/tiles/PlateL.png";
					else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
						spritePath = "level/tiles/PlateR.png";
					else
						spritePath = "level/tiles/PlateM.png";

					auto& go = AddTileGameObject(spritePath, { x,y }, { offsetX + pieceOffset, offsetY + pieceOffset });
					go.tag = "Plate";
					const auto coll = go.AddComponent<kob::ColliderComponent>();
					coll->SetHalfSize({ m_TileSize / 2, m_TileSize / 8, 1 });
					coll->offset = { 0, m_TileSize / 3, 1 };

				}
				break;
			}
			case TileType::Ladder:
			{
				AddTileGameObject("level/tiles/Ladder.png", { x,y }, { tileSize / 2, tileSize / 2 });
				break;
			}
			case TileType::LadderPlatform:
			{
				auto& go = AddTileGameObject(GetLadderPlatformPath(x), { x,y }, { tileSize / 2, tileSize / 2 });
				go.tag = "Platform";
				break;
			}
			case TileType::BottomBun:
			{
				if (GetTileSubType(x, y, TileType::BottomBun) == -1)
					AddIngredientTile(TileType::BottomBun, "level/tiles/BottomBun", x, y);
				break;
			}
			case TileType::TopBun:
			{
				if (GetTileSubType(x, y, TileType::TopBun) == -1)
					AddIngredientTile(TileType::TopBun, "level/tiles/TopBun", x, y);
				break;
			}
			case TileType::Lettuce:
			{
				if (GetTileSubType(x, y, TileType::Lettuce) == -1)
					AddIngredientTile(TileType::Lettuce, "level/tiles/Lettuce", x, y);
				break;
			}
			case TileType::Burger:
			{
				if (GetTileSubType(x, y, TileType::Burger) == -1)
					AddIngredientTile(TileType::Burger, "level/tiles/Burger", x, y);
				break;
			}
			case TileType::Tomato:
			{
				if (GetTileSubType(x, y, TileType::Tomato) == -1)
					AddIngredientTile(TileType::Tomato, "level/tiles/Tomato", x, y);
				break;
			}
			case TileType::Cheese:
			{
				if (GetTileSubType(x, y, TileType::Cheese) == -1)
					AddIngredientTile(TileType::Cheese, "level/tiles/Cheese", x, y);
				break;
			}
			case TileType::HiddenLadder:
			case TileType::Empty:
			{
				break;
			}
			case TileType::SpawnHotDog:
			{
				SpawnEnemy("HotDog", "characters/HotDogSheet.png", { x,y });
				break;
			}
			case TileType::SpawnEgg:
			{
				SpawnEnemy("Egg", "characters/EggSheet.png", { x,y });
				break;
			}
			case TileType::SpawnPickle:
			{
				SpawnEnemy("Pickle", "characters/PickleSheet.png", { x,y });
				break;
			}
			default:
			{
				if (IsLadderAbove(x, y))
					AddPlatformTile(GetLadderPlatformPath(x), { x,y }, true);
				else
					AddPlatformTile(GetPlatformPath(x), { x,y }, true);
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

bool bt::LevelComponent::CanMoveTo(uint32_t col, uint32_t row, bool isEnemy) const
{
	if (col >= m_Cols || row >= m_Rows)
		return false;

	const auto tile = m_vTiles[col + row * m_Cols];
	const bool playerAllowed = tile == TileType::Ladder
		|| tile == TileType::LadderPlatform

		|| tile == TileType::Platform

		|| tile == TileType::TopBun
		|| tile == TileType::Tomato
		|| tile == TileType::Lettuce
		|| tile == TileType::Cheese
		|| tile == TileType::Burger
		|| tile == TileType::BottomBun

		|| tile == TileType::SpawnChef;
	const bool enemyAllowed = playerAllowed ||
		tile == TileType::HiddenLadder ||
		tile == TileType::Plate;

	return isEnemy ? enemyAllowed : playerAllowed;
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


//--------------------------------------------------
//    Tile Spawning
//--------------------------------------------------
kob::GameObject& bt::LevelComponent::AddTileGameObject(const std::string& texturePath, const glm::uvec2& xy, const glm::vec2& offset) const
{
	auto& scene = GetGameObject()->GetScene();
	auto& tileObj = scene.AddEmpty();
	tileObj.SetParent(GetGameObject());
	tileObj.SetLocalPosition({ xy.x * m_TileSize + offset.x, xy.y * m_TileSize + offset.y, 0 });
	tileObj.SetLocalScale({ 2, 2, 2 });
	if (!texturePath.empty())
		tileObj.AddComponent<kob::ImageRendererComponent>(texturePath);
	return tileObj;
}
void bt::LevelComponent::AddPlatformTile(const std::string& texturePath, const glm::uvec2& xy, bool giveCollider) const
{
	auto& go = AddTileGameObject(texturePath, xy, { m_TileSize / 2, m_TileSize / 2 });
	if (giveCollider)
	{
		go.tag = "Platform";
		auto coll = go.AddComponent<kob::ColliderComponent>();
		coll->SetHalfSize({ m_TileSize / 2, m_TileSize / 8, 1 });
		coll->offset = { 0, m_TileSize / 2 + m_TileSize / 8, 1 };
	}
}
void bt::LevelComponent::AddIngredientTile(TileType type, const std::string& basePath, uint32_t x, uint32_t y) const
{
	uint32_t col = x;
	auto tile = m_vTiles[y * m_Cols + col];

	// spawn parent object that will handle the child ingredients
	auto& scene = GetGameObject()->GetScene();
	auto& parentObj = scene.AddEmpty();
	parentObj.SetParent(GetGameObject());
	parentObj.AddComponent<IngredientComponent>();
	parentObj.tag = "Ingredient";
	std::vector<kob::GameObject*> vChildren{};
	vChildren.reserve(6); // pre-reserve 6, since, unless doing something special, ingredient are per 6

	// spawn all ingredient tiles
	const float pieceSize = m_TileSize / 2;
	const float pieceOffset = m_TileSize / 4;
	while (tile == type && col < m_Cols)
	{
		if (IsLadderAbove(col, y))
			AddPlatformTile(GetLadderPlatformPath(col), { col,y }, true);
		else
			AddPlatformTile(GetPlatformPath(col), { col,y }, true);

		std::string spritePath{};
		for (int i{}; i < 2; ++i)
		{
			const float offsetX = static_cast<float>(i % 2) * pieceSize;
			const float offsetY = pieceSize;

			int subType = GetTileSubType(col, y, type);
			if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
				spritePath = basePath + "L.png";
			else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
				spritePath = basePath + "R.png";
			else if (abs(offsetX) >= FLT_EPSILON)
				spritePath = basePath + "M1.png";
			else
				spritePath = basePath + "M2.png";

			auto& obj = AddTileGameObject(spritePath, { col,y }, { offsetX + pieceOffset, offsetY + pieceOffset });
			auto collider = obj.AddComponent<kob::ColliderComponent>();
			collider->SetHalfSize({ pieceSize / 2.f, pieceSize / 2.f , 1 });
			obj.AddComponent<IngredientTileComponent>();
			obj.SetRenderPriority(49);
			vChildren.push_back(&obj);
		}
		++col;
		tile = m_vTiles[y * m_Cols + col];
	}

	glm::vec3 pos = glm::vec3(0);
	for (const auto& c : vChildren)
		pos += c->GetWorldTransform().GetPosition();
	pos /= vChildren.size();
	parentObj.SetLocalPosition(pos);
	for (const auto& c : vChildren)
		c->SetParent(&parentObj, true);
	const auto collider = parentObj.AddComponent<kob::ColliderComponent>();
	collider->SetHalfSize({ pieceSize * static_cast<int>(parentObj.GetChildCount()), pieceSize, 1.f });
}
void bt::LevelComponent::SpawnChef() const
{
	constexpr float chefWalkDelay = 0.1f;
	constexpr float chefDeathDelay = 0.1f;
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
			        }, chefWalkDelay} },
		        {"Death", {
			        {
			            {48, 32, chefTxtSize, chefTxtSize},
			            {64, 32, chefTxtSize, chefTxtSize},
			            {80, 32, chefTxtSize, chefTxtSize},
			            {96, 32, chefTxtSize, chefTxtSize},
			            {112, 32, chefTxtSize, chefTxtSize},
			            {128, 32, chefTxtSize, chefTxtSize},
			        }, chefDeathDelay} }
	        });

	// spawn go
	auto& scene = GetGameObject()->GetScene();

	// init chef
	auto& chef = scene.AddEmpty("Player");
	chef.SetParent(GetGameObject());
	chef.SetRenderPriority(48);
	chef.tag = "Player";

	// add components
	const auto chefHealth = chef.AddComponent<HealthComponent>(4);
	chefHealth->OnDamageTaken() += []
	{
		kob::ServiceLocator::GetSoundService().Pause("sound/BGM.wav");
		kob::ServiceLocator::GetSoundService().Play("sound/Death.wav", 0.25f, 0);
	};
	const auto chefScore = chef.AddComponent<ScoreComponent>();
	const auto renderComp = chef.AddComponent<kob::ImageRendererComponent>(chefSheet->GetTexture());
	const auto animator = chef.AddComponent<kob::Animator>(renderComp, chefSheet);
	const auto chefMovement = chef.AddComponent<MovementComponent>(speed);
	chefMovement->SetCurrentLevel(*this);
	animator->Play("Down", false);

	// set spawn
	auto chefSpawn = GetChefSpawn();
	chef.SetLocalPosition({ chefSpawn.x, chefSpawn.y, 0 });
	chef.SetLocalScale(glm::vec3(2, 2, 2));

	// Add collider
	auto collider = chef.AddComponent<kob::ColliderComponent>();
	collider->SetHalfSize({ chefTxtSize * 3.f / 4.f, chefTxtSize, chefTxtSize });

	// pepper
	auto chefPepper = chef.AddComponent<ThrowPepperComponent>(5);

	// input
	auto& inputManager = kob::InputManager::GetInstance();
	inputManager.RegisterKeyboardCmd(SDLK_w, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0, -1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_s, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0,  1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_d, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 1,  0, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_a, kob::TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ -1,  0, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_SPACE, kob::TriggerState::Pressed, std::make_unique<ThrowPepperCommand>(*chefMovement, *chefPepper, m_TileSize));

	inputManager.RegisterKeyboardCmd(SDLK_c, kob::TriggerState::Pressed, std::make_unique<DamageCommand>(*chefHealth));
	inputManager.RegisterKeyboardCmd(SDLK_z, kob::TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 10));
	inputManager.RegisterKeyboardCmd(SDLK_x, kob::TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 100));
}
void bt::LevelComponent::SpawnEnemy(const std::string& name, const std::string& sheetPath, const glm::uvec2& xy) const
{
	constexpr float walkDelay = 0.1f;
	constexpr float deathDelay = 0.1f;
	constexpr float stunDelay = 0.25f;
	constexpr int txtSize = 16;
	constexpr float speed = 40.f;
	auto sheet = kob::ResourceManager::GetInstance().LoadSpriteSheet(sheetPath,
		{
			{"Down", {
				{
					{32, 0, txtSize, txtSize},
					{48, 0, txtSize, txtSize},
				}, walkDelay} },
			{"Up", {
				{
					{32, 16, txtSize, txtSize},
					{48, 16, txtSize, txtSize},
				}, walkDelay} },
			{"Left", {
				{
					{0, 0, txtSize, txtSize},
					{16, 0, txtSize, txtSize},
				}, walkDelay} },
			{"Right", {
				{
					{0, 16, txtSize, txtSize},
					{16, 16, txtSize, txtSize},
				}, walkDelay} },
			{"Squashed", {
				{
					{0, 32, txtSize, txtSize},
					{16, 32, txtSize, txtSize},
					{32, 32, txtSize, txtSize},
					{48, 32, txtSize, txtSize},
				}, deathDelay} },
			{"Stunned", {
				{
					{64, 32, txtSize, txtSize},
					{64, 16, txtSize, txtSize},
				}, stunDelay} }
		});

	// spawn go
	auto& scene = GetGameObject()->GetScene();

	// init chef
	auto& enemy = scene.AddEmpty(name);
	enemy.SetParent(GetGameObject());
	enemy.SetRenderPriority(48);
	enemy.tag = "Enemy";

	// add components
	enemy.AddComponent<EnemyAILogicComponent>();
	enemy.AddComponent<SquashableComponent>();
	enemy.AddComponent<StunnableComponent>();
	const auto renderComp = enemy.AddComponent<kob::ImageRendererComponent>(sheet->GetTexture());
	const auto animator = enemy.AddComponent<kob::Animator>(renderComp, sheet);
	const auto movement = enemy.AddComponent<MovementComponent>(speed, true);
	movement->SetCurrentLevel(*this);
	animator->Play("Down", false);

	// set spawn
	auto spawn = ColRowToCenterPos(xy);
	enemy.SetLocalPosition({ spawn.x, spawn.y, 0 });
	enemy.SetLocalScale(glm::vec3(2, 2, 2));

	// Add collider
	auto collider = enemy.AddComponent<kob::ColliderComponent>();
	collider->SetHalfSize({ txtSize * 3.f / 4.f, txtSize, txtSize });

	// cover up
	auto& cover = scene.AddEmpty("Cover");
	cover.SetParent(GetGameObject());
	cover.SetRenderPriority(47);
	cover.SetLocalPosition({ spawn.x, spawn.y, 0 });
	const auto black = cover.AddComponent<kob::ImageRendererComponent>("level/tiles/black.png");
	const auto bSize = black->GetSize();
	cover.SetLocalScale(glm::vec3(m_TileSize / bSize.x, m_TileSize / bSize.y, 1));
}
