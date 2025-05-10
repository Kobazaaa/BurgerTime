#include "LevelComponent.h"
#include "SceneManager.h"
#include "LevelLoader.h"

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
void bt::LevelComponent::Build(float tileSize)
{
	m_TileSize = tileSize;
	LevelLoader::SpawnTileMap(m_vTiles, m_Rows, m_Cols, *kob::SceneManager::GetInstance().GetScene(0), m_TileSize);
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
