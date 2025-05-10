#pragma once
#include <vector>
#include <fstream>
#include "vec2.hpp"
#include "LevelComponent.h"

namespace kob {	class Scene; }
namespace bt
{
	class LevelLoader
	{
	public:
		//--------------------------------------------------
		//    Loaders
		//--------------------------------------------------
		static std::vector<LevelComponent::TileType> LoadCSV(const std::filesystem::path& file, uint32_t& outRows, uint32_t& outCols);
		static void SpawnTileMap(const std::vector<LevelComponent::TileType>& tiles, uint32_t rows, uint32_t cols, kob::Scene& scene, float tileSize);

	private:
		//--------------------------------------------------
		//    Information
		//--------------------------------------------------
		using TileType = LevelComponent::TileType;
		static int GetTileSubType(const std::vector<TileType>& tiles, uint32_t x, uint32_t y, uint32_t cols, TileType type);
		static bool IsLadderAbove(const std::vector<TileType>& tiles, uint32_t x, uint32_t y, uint32_t cols, uint32_t rows);

		static std::string GetPlatformPath(uint32_t x);
		static std::string GetLadderPlatformPath(uint32_t x);

		static void AddTileGameObject(kob::Scene& scene, const std::string& texturePath, const glm::uvec2& xy, float tileSize, const glm::vec2& offset = {});
		static void AddIngredientTile(TileType type, const std::vector<TileType>& tiles, kob::Scene& scene, const std::string& basePath, uint32_t x, uint32_t y, uint32_t cols, uint32_t rows, float tileSize);
	};



}
