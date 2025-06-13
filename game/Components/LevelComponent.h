#pragma once
#include <filesystem>
#include "Component.h"
#include "glm.hpp"
#include "Scene.h"

namespace bt
{
	class LevelComponent : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit LevelComponent(kob::GameObject& parent, const std::filesystem::path& levelPath);
		void SpawnTileMap(float tileSize = 32.f);
		kob::GameObject* SpawnChef(const std::string& name, const std::string& sheetPath) const;

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override {}

		//--------------------------------------------------
		//    Enum
		//--------------------------------------------------
		enum class TileType
		{
			// Level Tiles
			Empty,
			Platform,
			Plate,
			HiddenLadder,
			Ladder,
			LadderPlatform,

			// Ingredient Tiles
			BottomBun,
			TopBun,
			Lettuce,
			Burger,
			Tomato,
			Cheese,

			// Spawn Point Tiles
			SpawnHotDog,
			SpawnEgg,
			SpawnPickle,
			SpawnChef
		};


		//--------------------------------------------------
		//    Helpers
		//--------------------------------------------------
		uint32_t PosToIdx(const glm::vec2& pos) const;
		uint32_t PosToIdx(const glm::vec3& pos) const;
		glm::uvec2 PosToColRow(const glm::vec2& pos) const;
		glm::uvec2 PosToColRow(const glm::vec3& pos) const;
		glm::vec2 IdxToCenterPos(uint32_t idx) const;
		glm::vec2 ColRowToCenterPos(const glm::uvec2& colRow) const;

		bool IsIngredientTile(TileType tile) const;
		bool IsAlignedVertically(const glm::vec2& pos, float threshold) const;
		bool IsAlignedHorizontally(const glm::vec2& pos, float threshold) const;
		bool CanMoveTo(uint32_t col, uint32_t row, bool isEnemy) const;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		glm::vec2 GetChefSpawn() const;
		TileType GetTileType(uint32_t idx) const;
		TileType GetTileType(const glm::uvec2& colRow) const;

		glm::uvec2 GetColRow() const;
		float GetTileSize() const;

	private:
		//--------------------------------------------------
		//    Information
		//--------------------------------------------------
		int GetTileSubType(uint32_t x, uint32_t y, TileType type) const;
		bool IsLadderAbove(uint32_t x, uint32_t y) const;

		std::string GetPlatformPath(uint32_t x) const;
		std::string GetLadderPlatformPath(uint32_t x) const;

		//--------------------------------------------------
		//    Tile Spawning
		//--------------------------------------------------
		kob::GameObject& AddTileGameObject(const std::string& texturePath, const glm::uvec2& xy, const glm::vec2& offset) const;
		void AddPlatformTile(const std::string& texturePath, const glm::uvec2& xy, bool giveCollider) const;
		void AddIngredientTile(TileType type, const std::string& basePath, uint32_t x, uint32_t y) const;

		kob::GameObject* SpawnEnemy(const std::string& name, const std::string& sheetPath, const glm::uvec2& xy) const;

		// Data 
		uint32_t m_Rows{};
		uint32_t m_Cols{};
		std::vector<TileType> m_vTiles;
		float m_TileSize{ 32.f };

		uint32_t m_ChefSpawn{};

		kob::GameObject* m_pPlayer{};
	};
}
