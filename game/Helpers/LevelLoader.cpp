#include "LevelLoader.h"
#include "ImageRendererComponent.h"
#include "Scene.h"

#include <filesystem>
#include <string>
#include <unordered_map>


//--------------------------------------------------
//    Loaders
//--------------------------------------------------
std::vector<bt::LevelLoader::TileType>
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
void bt::LevelLoader::SpawnTileMap(const std::vector<TileType>& tiles, uint32_t rows, uint32_t cols, kob::Scene& scene,	float tileSize)
{
	for (uint32_t y{}; y < rows; ++y)
	{
		for (uint32_t x{}; x < cols; ++x)
		{
			std::string spritePath{};
			switch (tiles[y * cols + x])
			{
			case TileType::Platform:
			{
				spritePath = GetPlatformPath(x);
				AddTileGameObject(scene, spritePath, { x,y }, tileSize);
				break;
			}
			case TileType::Plate:
			{
				const float pieceSize = tileSize / 2;
				for (int i{}; i < 2; ++i)
				{
					float offsetX = static_cast<float>(i % 2) * pieceSize;
					float offsetY = 16.f;

					int subType = GetTileSubType(tiles, x, y, cols, TileType::Plate);
					if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
						spritePath = "level/tiles/PlateL.png";
					else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
						spritePath = "level/tiles/PlateR.png";
					else
						spritePath = "level/tiles/PlateM.png";

					AddTileGameObject(scene, spritePath, { x,y }, tileSize, { offsetX, offsetY });
				}
				break;
			}
			case TileType::Ladder:
			{
				spritePath = "level/tiles/Ladder.png";
				AddTileGameObject(scene, spritePath, { x,y }, tileSize);
				break;
			}
			case TileType::LadderPlatform:
			{
				spritePath = GetLadderPlatformPath(x);
				AddTileGameObject(scene, spritePath, { x,y }, tileSize);
				break;
			}
			case TileType::BottomBun:
			{
				AddIngredientTile(TileType::BottomBun, tiles, scene, "level/tiles/BottomBun", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::TopBun:
			{
				AddIngredientTile(TileType::TopBun, tiles, scene, "level/tiles/TopBun", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::Lettuce:
			{
				AddIngredientTile(TileType::Lettuce, tiles, scene, "level/tiles/Lettuce", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::Burger:
			{
				AddIngredientTile(TileType::Burger, tiles, scene, "level/tiles/Burger", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::Tomato:
			{
				AddIngredientTile(TileType::Tomato, tiles, scene, "level/tiles/Tomato", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::Cheese:
			{
				AddIngredientTile(TileType::Cheese, tiles, scene, "level/tiles/Cheese", x, y, cols, rows, tileSize);
				break;
			}
			case TileType::Empty:
			{
				break;
			}
			default:
			{
				if (IsLadderAbove(tiles, x, y, cols, rows))
				{
					spritePath = GetLadderPlatformPath(x);
					AddTileGameObject(scene, spritePath, { x,y }, tileSize);
				}
				else
				{
					spritePath = GetPlatformPath(x);
					AddTileGameObject(scene, spritePath, { x,y }, tileSize);
				}
				break;
			}
			}
		}
	}
}


//--------------------------------------------------
//    Information
//--------------------------------------------------
int bt::LevelLoader::GetTileSubType(const std::vector<TileType>& tiles, uint32_t x, uint32_t y, uint32_t cols, TileType type)
{
	uint32_t leftCol = x - 1;
	uint32_t rightCol = x + 1;

	if (leftCol == 0xFFFFFFFF)
		return -1;
	if (rightCol >= cols)
		return 1;

	const uint32_t neighbourIdxL = y * cols + leftCol;
	const uint32_t neighbourIdxR = y * cols + rightCol;

	if (tiles[neighbourIdxL] == type &&
		tiles[neighbourIdxR] == type)
		return 0;

	if (tiles[neighbourIdxL] == type
		&& tiles[neighbourIdxR] != type)
		return 1;

	if (tiles[neighbourIdxL] != type
		&& tiles[neighbourIdxR] == type)
		return -1;

	return 0;
}
bool bt::LevelLoader::IsLadderAbove(const std::vector<TileType>& tiles, uint32_t x, uint32_t y, uint32_t cols, uint32_t rows)
{
	uint32_t upRow = y - 1;
	if (upRow >= rows)
		return false;

	uint32_t neighbourIdxU = upRow * cols + x;
	if (tiles[neighbourIdxU] == TileType::Ladder || tiles[neighbourIdxU] == TileType::LadderPlatform)
		return true;
	return false;
}

std::string bt::LevelLoader::GetPlatformPath(uint32_t x)
{
	if (x % 4 == 0)
		return "level/tiles/PlatformLight.png";
	if (x % 2 == 0)
		return "level/tiles/PlatformDark2.png";
	return "level/tiles/PlatformDark.png";
}
std::string bt::LevelLoader::GetLadderPlatformPath(uint32_t x)
{
	if (x % 4 == 0)
		return "level/tiles/LadderPlatformLight.png";
	return "level/tiles/LadderPlatformDark.png";
}

void bt::LevelLoader::AddTileGameObject(kob::Scene& scene, const std::string& texturePath, const glm::uvec2& xy, float tileSize, const glm::vec2& offset)
{
	auto& tileObj = scene.AddEmpty();
	tileObj.SetLocalPosition({ xy.x * tileSize + offset.x, xy.y * tileSize + offset.y, 0 });
	tileObj.SetLocalScale({ 2, 2, 2 });
	if (!texturePath.empty())
		tileObj.AddComponent<kob::ImageRendererComponent>(texturePath);
}
void bt::LevelLoader::AddIngredientTile(TileType type, const std::vector<TileType>& tiles, kob::Scene& scene, const std::string& basePath, uint32_t x, uint32_t y, uint32_t cols, uint32_t rows, float tileSize)
{
	std::string spritePath{};
	if (IsLadderAbove(tiles, x, y, cols, rows))
	{
		spritePath = GetLadderPlatformPath(x);
		AddTileGameObject(scene, spritePath, { x,y }, tileSize);
	}
	else
	{
		spritePath = GetPlatformPath(x);
		AddTileGameObject(scene, spritePath, { x,y }, tileSize);
	}

	const float pieceSize = tileSize / 2;
	for (int i{}; i < 2; ++i)
	{
		float offsetX = static_cast<float>(i % 2) * pieceSize;
		float offsetY = pieceSize;

		int subType = GetTileSubType(tiles, x, y, cols, type);
		if (subType == -1 && abs(offsetX) <= FLT_EPSILON)
			spritePath = basePath + "L.png";
		else if (subType == 1 && abs(offsetX) >= FLT_EPSILON)
			spritePath = basePath + "R.png";
		else if (abs(offsetX) >= FLT_EPSILON)
			spritePath = basePath + "M1.png";
		else
			spritePath = basePath + "M2.png";

		AddTileGameObject(scene, spritePath, { x,y }, tileSize, { offsetX, offsetY });
	}
}
