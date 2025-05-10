#pragma once
#include "Component.h"
#include "LevelLoader.h"
#include "vec3.hpp"

namespace bt
{
	class LevelComponent : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit LevelComponent(kob::GameObject& parent, const std::filesystem::path& levelPath);
		void Build(float tileSize = 32.f);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override{};
		void Update() override{};


		//--------------------------------------------------
		//    Helpers
		//--------------------------------------------------
		uint32_t PosToIdx(const glm::vec2& pos) const;
		uint32_t PosToIdx(const glm::vec3& pos) const;
		glm::uvec2 PosToRowCol(const glm::vec2& pos) const;
		glm::uvec2 PosToRowCol(const glm::vec3& pos) const;
		glm::vec2 IdxToPos(uint32_t idx) const;
		glm::vec2 RowColToPos(glm::uvec2 rowCol) const;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		glm::vec2 GetChefSpawn() const;


	private:
		uint32_t m_Rows{};
		uint32_t m_Cols{};
		std::vector<LevelLoader::TileType> m_vTiles;
		float m_TileSize{ 32.f };

		uint32_t m_ChefSpawn{};
	};
}
