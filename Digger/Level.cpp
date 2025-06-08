#include "Level.h"
#include "DiggerConstants.h"
#include "Scene.h"
#include "GameObject.h"
#include "TerrainComponent.h"

#include <iostream>
#include <fstream>
#include "PrototypeSpawner.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Emerald.h"
#include "MoneyBag.h"
#include "Gold.h"

Level::Level(Rinigin::Scene* scene) :
	m_Scene(scene)
{
	const glm::vec2 SCREEN_SIZE{ DIGGER::SCREEN_WIDTH, DIGGER::SCREEN_HEIGHT };
	m_LevelGameObject = scene->CreateObject();
	m_MapComponent = m_LevelGameObject->AddComponent<TerrainComponent>(SCREEN_SIZE, SCREEN_SIZE, 40);


	m_ItemSpawner.RegisterPrototype<Emerald>("Emerald",this);
	m_ItemSpawner.RegisterPrototype<MoneyBag>("MoneyBag", this);
	m_ItemSpawner.RegisterPrototype<Gold>("Gold", this);
}

const glm::vec2& Level::GetPlayerSpawnIndex(int playerIndex)
{
	return m_LevelData.playerSpawns[playerIndex % m_LevelData.playerSpawns.size()];
}

void Level::LoadLevel(int levelIndex)
{
	const std::string levelName{ "LVL" + std::to_string(levelIndex) };
	std::string levelResourcesPath{ "../Data/Levels/" };
	std::string levelTexturePath{ levelResourcesPath + "Textures/" + levelName + ".png" };
	std::string levelDataPath{ levelResourcesPath + "Data/" + levelName + ".json" };

	// Load Texture

	m_MapComponent->ChangeBackgroundTexture(levelTexturePath.c_str());
	LoadLevelFile(levelDataPath.c_str());
	InitializeLevel();
}

void Level::LoadLevelFile(const char* filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) {
		std::cerr << "Failed to open " << filePath << "\n";
		return;
	}

	json j;
	ifs >> j;

	m_LevelData.width = j["width"];
	m_LevelData.height = j["height"];

	const auto& tilesJson = j["tiles"];
	int totalTileCount{ m_LevelData.width * m_LevelData.height };

	if (tilesJson.size() != totalTileCount) {
		std::cerr << "Tile count mismatch.\n";
		return;
	}

	m_LevelData.tiles.resize(totalTileCount);

	for (size_t tileIndex{ 0 }; tileIndex < tilesJson.size(); tileIndex++) {
		int tileValue = tilesJson[tileIndex];
		m_LevelData.tiles[tileIndex] = tileValue;
	}
}

void Level::InitializeLevel()
{
	const int tileWidth = static_cast<int>(DIGGER::SCREEN_WIDTH / m_LevelData.width);
	const int tileHeight = static_cast<int>(DIGGER::SCREEN_HEIGHT / m_LevelData.height);
	const int totalTiles = (m_LevelData.width * m_LevelData.height);
	std::vector<glm::vec2> tunnelPositions{};

	for (int tileIndex{ 0 }; tileIndex < totalTiles; tileIndex++) {
		const unsigned int tileValue = m_LevelData.tiles[tileIndex];

		// Reconstruct 2D position from 1D index
		const int x = tileIndex % m_LevelData.width;
		const int y = tileIndex / m_LevelData.width;
		const glm::vec2 tilePos{ x * tileWidth, y * tileHeight };

		switch (tileValue)
		{
		case 0:
			// Default nothing
			break;
		case 1:
			// Dug out
			//m_MapComponent->DigAt(tilePos.x, tilePos.y, DIGGER::TILE_SIZE);
			tunnelPositions.emplace_back(tilePos);
			break;
		case 2:
			// Spawn points
			tunnelPositions.emplace_back(tilePos);
			m_LevelData.playerSpawns.emplace_back(tilePos);
			//m_MapComponent->DigAt(tilePos.x, tilePos.y, DIGGER::TILE_SIZE);
			break;
		case 3: {
			// Emeralds
			
			Emerald* emerald = static_cast<Emerald*>(m_ItemSpawner.Spawn("Emerald"));
			emerald->GetItemObject()->SetPosition(glm::vec2(tilePos.x, tilePos.y));
			// TODO : Add score

			// TODO : Spawn emeralds
			/*Emerald* emerald = CreateItem<Emerald>(glm::vec2(tilePos.x, tilePos.y));
			emerald->GetCollectedEvent()->AddObserver(m_DiggerOne->GetScoreComponent());
			emerald->GetCollectedEvent()->AddObserver(m_DiggerTwo->GetScoreComponent());*/
			break;
		}
		case 4:
			// Enemy spawn
			tunnelPositions.emplace_back(tilePos);
			break;
		case 5: {
			// Money bag

			MoneyBag* moneyBag = static_cast<MoneyBag*>(m_ItemSpawner.Spawn("MoneyBag"));
			moneyBag->GetItemObject()->SetPosition(glm::vec2(tilePos.x, tilePos.y));
			//CreateItem<MoneyBag>(glm::vec2(tilePos.x, tilePos.y), m_MapComponent);
			break;
		}
		default:
			break;
		}
	}

	// Complete the tunnels
	//std::sort(tunnelPositions.begin(), tunnelPositions.end(), [](const glm::vec2& a, const glm::vec2& b) {
	//		if (a.x == b.x) return a.y < b.y; // Top to bottom
	//		return a.x < b.x;     // Left to right
	//	}
	//);

	tunnelPositions = SortTunnelByChasingClosest(tunnelPositions);
	for (int tunnelIndex{ 0 }; tunnelIndex < tunnelPositions.size(); tunnelIndex++) {
		if (tunnelIndex <= 0) continue;

		const glm::vec2& prevPosition{ tunnelPositions[tunnelIndex - 1] };
		const glm::vec2& tunnelPos{ tunnelPositions[tunnelIndex] };

		for (int sampleIndex{ 0 }; sampleIndex < 20; sampleIndex++) {
			const float t{ static_cast<float>(sampleIndex) / static_cast<float>(20) };
			
			const glm::vec2 interpolatedTilePos{  
				prevPosition.x + (tunnelPos.x - prevPosition.x) * t,
				prevPosition.y + (tunnelPos.y - prevPosition.y) * t,
			};
			
			m_MapComponent->DigAt(interpolatedTilePos.x, interpolatedTilePos.y, DIGGER::TILE_SIZE);
		}

	}
}

std::vector<glm::vec2> Level::SortTunnelByChasingClosest(std::vector<glm::vec2> positions)
{
	std::vector<glm::vec2> ordered;
	if (positions.empty()) return ordered;

	// Start with any point
	glm::vec2 current = positions.back();
	positions.pop_back();
	ordered.push_back(current);

	while (!positions.empty()) {
		auto closestIt = positions.begin();
		float minDist = glm::distance(current, *closestIt);

		for (auto it = positions.begin() + 1; it != positions.end(); ++it) {
			float dist = glm::distance(current, *it);
			if (dist < minDist) {
				minDist = dist;
				closestIt = it;
			}
		}

		current = *closestIt;
		ordered.push_back(current);
		positions.erase(closestIt);
	}

	return ordered;
}

//void Level::InitializeLevel()
//{
//	int tileWidth{ static_cast<int>(DIGGER::SCREEN_WIDTH / m_LevelData.width) };
//	int tileHeight{ static_cast<int>(DIGGER::SCREEN_HEIGHT / m_LevelData.height) };
//
//	glm::vec2 previousDugPosition{};
//
//	for (int x{ 0 }; x < m_LevelData.width; x++) {
//		for (int y{ 0 }; y < m_LevelData.height; y++) {
//
//			const int tileIndex{ y * m_LevelData.width + x };
//			const unsigned int tileValue{ m_LevelData.tiles[tileIndex] };
//			const glm::vec2 tilePos{ x * tileWidth, y * tileHeight };
//
//			switch (tileValue)
//			{
//			case 0:
//				// Default nothing
//				break;
//			case 1:
//				// Dug out
//				m_MapComponent->DigAt(tilePos.x, tilePos.y, DIGGER::TILE_SIZE);
//				
//				/*if (tileIndex > 0) {
//					for (int sampleIndex{ 0 }; sampleIndex < 20; sampleIndex++) {
//						const float t{ static_cast<float>(sampleIndex) / static_cast<float>(20) };
//
//						const glm::vec2 interpolatedTilePos{  
//							previousDugPosition.x + (tilePos.x - previousDugPosition.x) * t,
//							previousDugPosition.y + (tilePos.y - previousDugPosition.y) * t,
//						};
//
//						m_MapComponent->DigAt(interpolatedTilePos.x, interpolatedTilePos.y, DIGGER::TILE_SIZE);
//					}
//				}*/
//
//				previousDugPosition = tilePos;
//				break;
//			case 2:
//				// Spawn points
//				m_LevelData.playerSpawns.emplace_back(tilePos);
//				m_MapComponent->DigAt(tilePos.x, tilePos.y, DIGGER::TILE_SIZE);
//				break;
//			case 3: {
//				// Emeralds
//				// TODO : Spawn emeralds
//				/*Emerald* emerald = CreateItem<Emerald>(glm::vec2(tilePos.x, tilePos.y));
//				emerald->GetCollectedEvent()->AddObserver(m_DiggerOne->GetScoreComponent());
//				emerald->GetCollectedEvent()->AddObserver(m_DiggerTwo->GetScoreComponent());*/
//				break;
//			}
//			case 4:
//				// Enemy spawn
//			case 5:
//				// Money bag
//				// Spawn money bags
//				//CreateItem<MoneyBag>(glm::vec2(tilePos.x, tilePos.y), m_MapComponent);
//				break;
//			default:
//				break;
//			}
//
//		}
//	}
//}
