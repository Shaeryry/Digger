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
class Level;

enum class GameMode { 
	SOLO,
	COOP,
	PVP
};

class GameScreenState : public Rinigin::State
{
public:
	explicit GameScreenState(Rinigin::StateContextComponent* context);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
	
private:
	// TODO : Consider moving level logic into its own separate class

	/*struct LevelData{
		int width = 0;
		int height = 0;
		std::vector<glm::vec2> playerSpawns;
		std::vector<unsigned int> tiles;
	};

	glm::vec2& GetPlayerSpawnIndex(int playerIndex);

	LevelData m_LevelData;

	template<typename ItemType, typename... Args> ItemType* CreateItem(const glm::vec2& position = glm::vec2(), Args&&... arguments);
	std::vector<std::unique_ptr<Item>> m_Items;*/

	//

	void Reset();
	void StartSolo();
	void StartCoop();

	Rinigin::Scene* m_Scene;
	GameMode m_GameMode;

	//Rinigin::GameObject* m_BackgroundGameObject;
	//TerrainComponent* m_MapComponent;

	std::unique_ptr<DiggerMobile> m_DiggerOne;
	std::unique_ptr<DiggerMobile> m_DiggerTwo;
	Character* m_Nobbin; // TODO : Add nobbin character

	//std::vector< std::unique_ptr<Character> > m_Characters; 
	std::unique_ptr<Level> m_Level;
};
//
//template<typename ItemType,typename... Args>
//inline ItemType* GameScreenState::CreateItem(const glm::vec2& position, Args&&... arguments)
//{
//	std::unique_ptr<ItemType> createdItem = std::make_unique<ItemType>(std::forward<Args>(arguments)...);
//	createdItem->GetItemObject()->GetTransform()->SetLocalPosition(position);
//	m_Items.emplace_back(std::move(createdItem));
//	return dynamic_cast<ItemType*>(m_Items.back().get());
//}
