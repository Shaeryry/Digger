#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "PrototypeSpawner.h"
#include "Observer.h"

namespace Rinigin { class Scene; class GameObject; }
class TerrainComponent;
class Item;
class Nobbin;
class Character;

class Level final : public Rinigin::Observer
{
public:
	explicit Level(Rinigin::Scene* scene);
	virtual ~Level() = default;
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other) = delete;

	void LoadLevel(int levelIndex);
	void AddPlayer(Character* playerCharacter);
	void AddDeadPlayer(Character* player);
	void RemoveDeadPlayer(Character* player);
	Character* GetPlayer(int index) const { return m_Players[index]; };
	std::vector<Character*>& GetPlayers() { return m_Players; };

	void RespawnPlayer(int playerIndex,bool isEnemy);
	const glm::vec2& GetPlayerSpawnIndex(int playerIndex);
	const glm::vec2 GetEnemySpawnLocation();

	TerrainComponent* Map() const { return m_MapComponent; };
	Rinigin::Scene* GetScene() const { return m_Scene; };

	PrototypeSpawner<Item>& GetItemSpawner() { return m_ItemSpawner; };
	PrototypeSpawner<Nobbin>& GetEnemySpawner() { return m_EnemySpawner; };

	const glm::vec2& LevelTileSize() { return m_LevelTileSize; };
	virtual void Notify(Rinigin::EventArguments& arguments) override;
 private:
	void LoadLevelFile(const char* filePath);
	void InitializeLevel();
	std::vector<glm::vec2> SortTunnel(const std::vector<glm::vec2>& positions);

	struct LevelData {
		int width = 0; 
		int height = 0;
		std::vector<glm::vec2> playerSpawns;
		std::vector<glm::vec2> enemySpawns;
		std::vector<unsigned int> tiles;
	};
	struct IVec2Hash {
		std::size_t operator()(const glm::ivec2& v) const {
			return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
		}
	};
	const std::vector<glm::ivec2> kDirections = {
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	LevelData m_LevelData;
	TerrainComponent* m_MapComponent;

	Rinigin::GameObject* m_LevelGameObject;
	Rinigin::Scene* m_Scene;
	glm::vec2 m_LevelTileSize;

	// Players
	std::vector<Character*> m_Players;
	std::vector<Character*> m_AlivePlayers;
	
	// Spawners
	PrototypeSpawner<Item> m_ItemSpawner;
	PrototypeSpawner<Nobbin> m_EnemySpawner;
	std::vector<Rinigin::GameObject*> m_EnemySpawnerObjects;
};

