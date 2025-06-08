#include "LevelLoader.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
using TileType = bt::LevelComponent::TileType;

//--------------------------------------------------
//    Loaders
//--------------------------------------------------
std::vector<TileType>
	bt::LevelLoader::LoadCSV(const std::filesystem::path& file, uint32_t& outRows, uint32_t& outCols)
{
	if (!exists(file))
		throw std::runtime_error("CSV file does not exist: " + file.string());

	std::vector<TileType> tiles;
	std::ifstream inFile(file);
	if (!inFile.is_open())
		throw std::runtime_error("Failed to open CSV file: " + file.string());

	std::unordered_map<std::string, TileType> tileMap = {
		{"", TileType::Empty},
		{"E", TileType::Empty},
		{"P", TileType::Platform},
		{"PL", TileType::Plate},
		{"HL", TileType::HiddenLadder},
		{"L", TileType::Ladder},
		{"LP", TileType::LadderPlatform},

		{"BB", TileType::BottomBun},
		{"TB", TileType::TopBun},
		{"LE", TileType::Lettuce},
		{"BU", TileType::Burger},
		{"TO", TileType::Tomato},
		{"CH", TileType::Cheese},

		{"SH", TileType::SpawnHotDog},
		{"SE", TileType::SpawnEgg},
		{"SP", TileType::SpawnPickle},
		{"SC", TileType::SpawnChef}
	};

	std::string line;
	outRows = 0;
	outCols = 0;
	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);
		std::string cell;
		uint32_t currentCols = 0;
		while (std::getline(ss, cell, ';'))
		{
			if (!tileMap.contains(cell))
				throw std::runtime_error("Unknown tile type in CSV: " + cell);
			tiles.push_back(tileMap[cell]);
			++currentCols;
		}

		if (outRows == 0)
			outCols = currentCols;
		else if (currentCols != outCols)
			throw std::runtime_error("Inconsistent column count in CSV");
		++outRows;
	}
	return tiles;
}
