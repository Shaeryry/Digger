#pragma once
#include "State.h"
#include <memory>
#include <vector>   
#include <glm/glm.hpp>

namespace Rinigin { class Scene; class Gamepad; class GameObject; class Command; struct Binding; }

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
	PVP,
	END
};

class GameScreenState : public Rinigin::State
{
public:
	explicit GameScreenState(Rinigin::StateContextComponent* context);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	void SetLevel(int levelIndex);
	bool NextLevel();
	Level* GetLevel() const { return m_Level.get(); }

	GameMode GetGameMode() const { return m_GameMode; }
	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; };
private:
	void SetupBindings();
	void RemoveBindings();

	void StartGame();
	void Reset();
	void StartSolo();
	void StartCoop();

	bool m_AllPlayersDead;
	bool m_TransitionLevel;
	float m_TransitionClock;

	// Commands
	Rinigin::Command* m_SkipLevelCommand;
	//

	GameMode m_GameMode;

	std::unique_ptr<DiggerMobile> m_DiggerOne;
	std::unique_ptr<DiggerMobile> m_DiggerTwo;
	Nobbin* m_Nobbin;

	int m_CurrentLevelIndex;
	std::unique_ptr<Level> m_Level;

	// Bindings

	// Keyboard
	Rinigin::Binding* m_KeyboardSkipLevel;

	Rinigin::Binding* m_KeyboardDirectionUp;
	Rinigin::Binding* m_KeyboardDirectionDown;
	Rinigin::Binding* m_KeyboardDirectionLeft;
	Rinigin::Binding* m_KeyboardDirectionRight;

	Rinigin::Binding* m_KeyboardMoveUp;
	Rinigin::Binding* m_KeyboardMoveDown;
	Rinigin::Binding* m_KeyboardMoveLeft;
	Rinigin::Binding* m_KeyboardMoveRight;

	// Gamepad one

	Rinigin::Binding* m_GamepadOneDirectionUp;
	Rinigin::Binding* m_GamepadOneDirectionDown;
	Rinigin::Binding* m_GamepadOneDirectionLeft;
	Rinigin::Binding* m_GamepadOneDirectionRight;

	Rinigin::Binding* m_GamepadOneMoveUp;
	Rinigin::Binding* m_GamepadOneMoveDown;
	Rinigin::Binding* m_GamepadOneMoveLeft;
	Rinigin::Binding* m_GamepadOneMoveRight;

	// Gamepad two

	Rinigin::Binding* m_GamepadTwoDirectionUp;
	Rinigin::Binding* m_GamepadTwoDirectionDown;
	Rinigin::Binding* m_GamepadTwoDirectionLeft;
	Rinigin::Binding* m_GamepadTwoDirectionRight;

	Rinigin::Binding* m_GamepadTwoMoveUp;
	Rinigin::Binding* m_GamepadTwoMoveDown;
	Rinigin::Binding* m_GamepadTwoMoveLeft;
	Rinigin::Binding* m_GamepadTwoMoveRight;
};