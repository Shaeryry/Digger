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
class Nobbin;

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
	void StartGame();
	void Reset();
	void StartSolo();
	void StartCoop();

	Rinigin::Scene* m_Scene;
	GameMode m_GameMode;

	std::unique_ptr<DiggerMobile> m_DiggerOne;
	std::unique_ptr<DiggerMobile> m_DiggerTwo;
	Nobbin* m_Nobbin;

	std::unique_ptr<Level> m_Level;
};