#pragma once
#include "State.h"
#include <memory>
#include <vector>   
#include <glm/glm.hpp>

namespace Rinigin { class Scene; class Gamepad; class GameObject; class Command; }

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
	void StartGame();
	void Reset();
	void StartSolo();
	void StartCoop();

	// Commands
	Rinigin::Command* m_SkipLevelCommand;
	//

	GameMode m_GameMode;

	std::unique_ptr<DiggerMobile> m_DiggerOne;
	std::unique_ptr<DiggerMobile> m_DiggerTwo;
	Nobbin* m_Nobbin;

	int m_CurrentLevelIndex;
	std::unique_ptr<Level> m_Level;
};