#pragma once
#include "State.h"
#include <memory>
#include <vector>   
#include <glm/glm.hpp>

namespace Rinigin { class Scene; class Gamepad; class GameObject; }

class Character;
class DiggerGame; 
class DiggerMobile;
class TerrainComponent;
class Item;


enum class GameMode { 
	SOLO,
	COOP,
	PVP
};

class GameScreenState : public Rinigin::State
{
public:
	explicit GameScreenState(Rinigin::StateContextComponent* context,DiggerGame* game);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
	
private:
	// TODO : Consider moving level logic into its own separate class

	struct LevelData{
		int width = 0;
		int height = 0;
		std::vector<glm::vec2> playerSpawns;
		std::vector<unsigned int> tiles;
	};

	glm::vec2& GetPlayerSpawnIndex(int playerIndex);

	LevelData m_LevelData;

	template<typename ItemType> ItemType* CreateItem(const glm::vec2& position = glm::vec2());
	std::vector<std::unique_ptr<Item>> m_Items;

	//

	void Reset();
	void LoadLevel(int levelIndex);
	void LoadLevelFile(const char* levelName);
	void InitializeLevel();

	void StartSolo();
	void StartCoop();
	Character* AddCharacter(Character* character);

	DiggerGame* m_Game;
	Rinigin::Scene* m_Scene;
	GameMode m_GameMode;

	Rinigin::GameObject* m_BackgroundGameObject;
	TerrainComponent* m_MapComponent;

	DiggerMobile* m_DiggerOne;
	DiggerMobile* m_DiggerTwo;
	Character* m_Nobbin; // TODO : Add nobbin character

	std::vector< std::unique_ptr<Character> > m_Characters; 
};

template<typename ItemType>
inline ItemType* GameScreenState::CreateItem(const glm::vec2& position)
{
	std::unique_ptr<ItemType> createdItem = std::make_unique<ItemType>();
	createdItem->GetItemObject()->GetTransform()->SetLocalPosition(position);
	m_Items.emplace_back(std::move(createdItem));
	return dynamic_cast<ItemType*>(m_Items.back().get());
}
