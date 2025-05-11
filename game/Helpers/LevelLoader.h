#pragma once
#include <vector>
#include "LevelComponent.h"

namespace bt
{
	class LevelLoader
	{
	public:
		//--------------------------------------------------
		//    Loaders
		//--------------------------------------------------
		static std::vector<LevelComponent::TileType> LoadCSV(const std::filesystem::path& file, uint32_t& outRows, uint32_t& outCols);
	};
}
