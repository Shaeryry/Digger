#pragma once
#include "State.h"
#include <memory>
#include <vector>

namespace Rinigin { class Scene; class Gamepad; }

class Character;
class Digger;

enum class GameMode {
	SOLO,
	COOP,
	PVP
};

class GameScreenState : public Rinigin::State
{
public:
	explicit GameScreenState(Rinigin::StateContextComponent* context, Rinigin::Gamepad* playerOneGamepad, Rinigin::Gamepad* playerTwoGamepad);
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;

	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
private:
	void Reset();
	void StartSolo();

	Character* AddCharacter(Character* character);

	Rinigin::Gamepad* m_PlayerOneGamepad;
	Rinigin::Gamepad* m_PlayerTwoGamepad;
		
	GameMode m_GameMode;
	Rinigin::Scene* m_Scene; 

	Digger* m_DiggerOne;
	Digger* m_DiggerTwo;
	Character* m_Nobbin; // TODO : Add nobbin character
};

