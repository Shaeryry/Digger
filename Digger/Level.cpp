#include "Level.h"
#include "DiggerConstants.h"
#include "Scene.h"
#include "GameObject.h"
#include "TerrainComponent.h"
#include "EventTypes.h"

#include <iostream>
#include <fstream>
#include "PrototypeSpawner.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Emerald.h"
#include "MoneyBag.h"
#include "Gold.h"

#include "Nobbin.h"
#include "EnemySpawnerComponent.h"

#include "ColliderComponent.h"
#include "DiggerConstants.h"
#include "HealthComponent.h"

#include "ScoreComponent.h"
#include "LetterTextComponent.h"
#include "LivesComponent.h"

#include "DiggerMobile.h"
#include "DiggerMobileDiggingState.h"

#include "StateContextComponent.h"
#include "LivesDisplayComponent.h"

#include <queue>
#include <unordered_set>

Level::Level(Rinigin::Scene* scene) :
	m_Scene(scene),
	m_LevelTileSize(),
	m_TotalScore(0)
{
	const glm::vec2 SCREEN_SIZE{ DIGGER::SCREEN_WIDTH, DIGGER::SCREEN_HEIGHT };
	const glm::vec2 MAP_SIZE{ DIGGER::GAME_WIDTH, DIGGER::GAME_HEIGHT };
	const glm::vec2 ORIGIN{ (SCREEN_SIZE.x - MAP_SIZE.x),(SCREEN_SIZE.y - MAP_SIZE.y) };

	m_LevelGameObject = scene->CreateObject();
	m_MapComponent = m_LevelGameObject->AddComponent<TerrainComponent>(ORIGIN,SCREEN_SIZE, MAP_SIZE);

	// Score display
	m_ScoreDisplayTextComponent = m_LevelGameObject->AddComponent<LetterTextComponent>(Rinigin::Helpers::GetFormattedScore(0).c_str());
	m_ScoreDisplayTextComponent->GetOwner()->SetPosition(10.f, 5.f, 0);

	// Spawners
	m_EnemySpawner.RegisterPrototype<Nobbin>("Nobbin",this);
	m_EmeraldSpawner.RegisterPrototype<Emerald>("Emerald",this);

	m_ItemSpawner.RegisterPrototype<MoneyBag>("MoneyBag", this);
	m_ItemSpawner.RegisterPrototype<Gold>("Gold", this);

	m_LivesComponent = m_LevelGameObject->AddComponent<LivesComponent>(DIGGER::DIGGER_LIVES);

	m_LiveDisplayGameObject = m_Scene->CreateObject();
	m_LiveDisplayGameObject->SetParent(m_LevelGameObject);
	m_LiveDisplayGameObject->SetPosition( glm::vec3(m_ScoreDisplayTextComponent->GetLength() + 5,0,0) );
	m_LivesDisplayComponent = m_LiveDisplayGameObject->AddComponent<LivesDisplayComponent>(m_LivesComponent);


	UpdateScoreDisplay();
}

void Level::RespawnPlayer(int playerIndex,bool isEnemy)
{
	Character* character = GetPlayer(playerIndex);
	if (character) {
		character->GetCharacterObject()->SetActive(true);

		if (not isEnemy) {
			DiggerMobile* diggerMobile = dynamic_cast<DiggerMobile*>(character);
			diggerMobile->GetStateContext()->SetState(diggerMobile->GetDiggingState());
			character->GetHealthComponent()->GetDiedEvent()->AddObserver(this);
			character->GetCharacterObject()->SetPosition(GetPlayerSpawnIndex(playerIndex));
		}
		else {

		}

		RemoveDeadPlayer(character);
	}
}

const glm::vec2& Level::GetPlayerSpawnIndex(int playerIndex)
{
	return m_LevelData.playerSpawns[playerIndex % m_LevelData.playerSpawns.size()];
}

const glm::vec2 Level::GetEnemySpawnLocation()
{
	if (not m_LevelData.enemySpawns.empty()) {
		return m_LevelData.enemySpawns[rand() % m_LevelData.enemySpawns.size()];
	}

	return glm::vec2();
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

void Level::AddPlayer(Character* playerCharacter)
{
	ScoreComponent* scoreComponent{ playerCharacter->GetCharacterObject()->GetComponent<ScoreComponent>() };
	if (scoreComponent) scoreComponent->GetScoreAddedEvent()->AddObserver(this);
	m_Players.emplace_back(playerCharacter);
}

void Level::AddDeadPlayer(Character* player)
{
	auto foundIt = std::find(m_DeadPlayers.begin(), m_DeadPlayers.end(), player);
	if (foundIt != m_DeadPlayers.end()) return;

	m_DeadPlayers.emplace_back(player);
}

void Level::RemoveDeadPlayer(Character* player)
{
	std::erase(m_DeadPlayers, player);
}

void Level::Notify(Rinigin::EventArguments& arguments)
{
	switch (arguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("ScoreAdded"): {
		ScoreArguments& scoreAddedArguments{ GetArgumentsOfType<ScoreArguments>(arguments) };
		SetScore(m_TotalScore + scoreAddedArguments.GetScore());
		break;
	}
	default:
		break;
	}
}

void Level::CleanUpLevel()
{
	m_MapComponent->ResetPixels();

	m_EmeraldSpawner.ClearTracked();
	m_ItemSpawner.ClearTracked();
	m_EnemySpawner.ClearTracked();
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

	m_LevelData.playerSpawns.clear();
	m_LevelData.enemySpawns.clear();

	m_LevelData.tiles.clear();
	m_LevelData.tiles.resize(totalTileCount);

	for (size_t tileIndex{ 0 }; tileIndex < tilesJson.size(); tileIndex++) {
		int tileValue = tilesJson[tileIndex];
		m_LevelData.tiles[tileIndex] = tileValue;
	}
}

void Level::InitializeLevel()
{
	// TODO : Destroy and clean existing stuff
	CleanUpLevel();

	const int tileWidth = static_cast<int>(DIGGER::GAME_WIDTH / m_LevelData.width);
	const int tileHeight = static_cast<int>(DIGGER::GAME_HEIGHT / m_LevelData.height);
	m_LevelTileSize = glm::vec2(static_cast<float>(tileWidth),static_cast<float>(tileHeight));

	const int totalTiles = (m_LevelData.width * m_LevelData.height);
	std::vector<glm::vec2> tunnelPositions{};

	for (int tileIndex{ 0 }; tileIndex < totalTiles; tileIndex++) {
		const unsigned int tileValue = m_LevelData.tiles[tileIndex];

		// Reconstruct 2D position from 1D index
		const glm::vec2 position{ m_MapComponent->GetOrigin() };
		const int x =  (tileIndex % m_LevelData.width);
		const int y = tileIndex / m_LevelData.width;
		const glm::vec2 tilePos{ position + glm::vec2(x * tileWidth, y * tileHeight) };

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
			m_LevelData.playerSpawns.emplace_back(tilePos - glm::vec2(tileWidth/2.f, tileHeight /2.f));
			//m_MapComponent->DigAt(tilePos.x, tilePos.y, DIGGER::TILE_SIZE);
			break;
		case 3: {
			// Emeralds
			Emerald* emerald = m_EmeraldSpawner.Spawn("Emerald");
			emerald->GetItemObject()->SetPosition(glm::vec3(tilePos.x, tilePos.y, 0));
			break;
		}
		case 4: {
			// Enemy spawn
			m_MapComponent->DigAt(tilePos.x, tilePos.y, static_cast<int>(DIGGER::TILE_SIZE * 1.25f));

			Rinigin::GameObject* spawner = m_Scene->CreateObject();
			spawner->AddComponent<EnemySpawnerComponent>(this);
			m_EnemySpawnerObjects.emplace_back(spawner);

			tunnelPositions.emplace_back(tilePos);
			m_LevelData.enemySpawns.emplace_back(tilePos);
			break;
		}
			
		case 5: {
			// Money bag

			MoneyBag* moneyBag = static_cast<MoneyBag*>(m_ItemSpawner.Spawn("MoneyBag"));
			moneyBag->GetItemObject()->SetPosition(glm::vec3(tilePos.x, tilePos.y, 0) - moneyBag->GetCollider()->GetHalfExtents());
			break;
		}
		default:
			break;
		}
	}

	tunnelPositions = SortTunnel(tunnelPositions);
	for (int tunnelIndex{ 0 }; tunnelIndex < tunnelPositions.size(); tunnelIndex++) {
		if (tunnelIndex <= 0) continue;

		const glm::vec2& prevPosition{ tunnelPositions[tunnelIndex - 1] };
		const glm::vec2& tunnelPos{ tunnelPositions[tunnelIndex] };

		for (int sampleIndex{ 0 }; sampleIndex < DIGGER::TUNNEL_SAMPLES; sampleIndex++) {
			const float t{ static_cast<float>(sampleIndex) / static_cast<float>(DIGGER::TUNNEL_SAMPLES) };
			
			const glm::vec2 interpolatedTilePos{  
				prevPosition.x + (tunnelPos.x - prevPosition.x) * t,
				prevPosition.y + (tunnelPos.y - prevPosition.y) * t,
			};
			
			m_MapComponent->DigAt(interpolatedTilePos.x, interpolatedTilePos.y, DIGGER::TILE_SIZE);
		}
	}
}

void Level::UpdateScoreDisplay()
{
	m_ScoreDisplayTextComponent->SetText(Rinigin::Helpers::GetFormattedScore(m_TotalScore).c_str());
}

std::vector<glm::vec2> Level::SortTunnel(const std::vector<glm::vec2>& cpositions)
{
	std::vector<glm::vec2> positions = cpositions;
	std::vector<glm::vec2> ordered;
	if (positions.empty()) return ordered;

	// Use the first tile as the starting point (editor drawing order)
	glm::vec2 current = positions.front();
	positions.erase(positions.begin());
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
