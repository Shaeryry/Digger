#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "PrototypeSpawner.h"


namespace Rinigin { class Scene; class GameObject; }
class TerrainComponent;
class Item;

class Level final
{
public:
	explicit Level(Rinigin::Scene* scene);
	virtual ~Level() = default;
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other) = delete;

	void LoadLevel(int levelIndex);
	const glm::vec2& GetPlayerSpawnIndex(int playerIndex);

	TerrainComponent* Map() const { return m_MapComponent; };
	Rinigin::Scene* GetScene() const { return m_Scene; };
	PrototypeSpawner<Item>& GetItemSpawner() { return m_ItemSpawner; };
private:
	void LoadLevelFile(const char* filePath);
	void InitializeLevel();
	std::vector<glm::vec2> SortTunnelByChasingClosest(std::vector<glm::vec2> positions);

	struct LevelData {
		int width = 0;
		int height = 0;
		std::vector<glm::vec2> playerSpawns;
		std::vector<unsigned int> tiles;
	};

	LevelData m_LevelData;
	TerrainComponent* m_MapComponent;

	Rinigin::GameObject* m_LevelGameObject;
	Rinigin::Scene* m_Scene;

	// Spawners
	PrototypeSpawner<Item> m_ItemSpawner;
};

