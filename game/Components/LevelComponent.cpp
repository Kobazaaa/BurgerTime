#include "LevelComponent.h"
#include "SceneManager.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::LevelComponent::LevelComponent(kob::GameObject& parent, const std::filesystem::path& levelPath)
	: Component(parent)
{
	m_vTiles = LevelLoader::LoadCSV(levelPath, m_Rows, m_Cols);

	for (uint32_t i{}; i < m_vTiles.size(); ++i)
	{
		if (m_vTiles[i] == LevelLoader::TileType::SpawnChef)
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
	auto rowCol = PosToRowCol(pos);
	return rowCol.y * m_Cols + rowCol.x;
}
uint32_t bt::LevelComponent::PosToIdx(const glm::vec3& pos) const
{
	return PosToIdx({pos.x, pos.y});
}
glm::uvec2 bt::LevelComponent::PosToRowCol(const glm::vec2& pos) const
{
	return {
		static_cast<int>(pos.x / m_TileSize),
		static_cast<int>(pos.y / m_TileSize)
	};
}
glm::uvec2 bt::LevelComponent::PosToRowCol(const glm::vec3& pos) const
{
	return PosToRowCol({pos.x, pos.y});
}
glm::vec2 bt::LevelComponent::IdxToPos(uint32_t idx) const
{
	const int x = idx % m_Cols;
	const int y = idx / m_Cols;
	return { x * m_TileSize, y * m_TileSize };
}
glm::vec2 bt::LevelComponent::RowColToPos(glm::uvec2 rowCol) const
{
	return IdxToPos(rowCol.x * m_Cols + rowCol.y);
}


//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
glm::vec2 bt::LevelComponent::GetChefSpawn() const
{
	return IdxToPos(m_ChefSpawn);
}
