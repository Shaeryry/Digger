#pragma once
#include "State.h"
#include <memory>
#include <vector>

namespace Rinigin { class Scene; class Gamepad; class GameObject; }

class Character;
class DiggerGame;
class DiggerMobile;
class DestructibleEnvironmentComponent;

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
	virtual void Update() override;
	virtual void Exit() override;

	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
	
private:
	void Reset();
	void LoadLevel();
	void StartSolo();
	Character* AddCharacter(Character* character);

	DiggerGame* m_Game;
	Rinigin::Scene* m_Scene;
	GameMode m_GameMode;

	Rinigin::GameObject* m_BackgroundGameObject;
	DestructibleEnvironmentComponent* m_MapComponent;


	DiggerMobile* m_DiggerOne;
	DiggerMobile* m_DiggerTwo;
	Character* m_Nobbin; // TODO : Add nobbin character

	std::vector< std::unique_ptr<Character> > m_Characters; 

};

