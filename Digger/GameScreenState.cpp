#include "GameScreenState.h"

#include <iostream>
#include <fstream>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "TextureRendererComponent.h"
#include "GameObject.h"
#include "Character.h"
#include "DiggerMobile.h"
#include "Gamepad.h"
#include "MovementCommands.h" 
#include "Scene.h"
#include "DamageCommand.h"
#include "TextComponent.h"
#include "TerrainComponent.h"
#include "DiggerGame.h"
#include "RigidbodyComponent.h"

#include "ScoreComponent.h"
#include "Emerald.h"
#include "MoneyBag.h"

#include "Level.h"
#include "PrototypeSpawner.h"
#include "Prototype.h"
#include "Nobbin.h"

#include "LivesComponent.h"
#include "GameCommands.h"
#include "DiggerConstants.h"

#include "StateContextComponent.h"
#include "ScoreRegisterScreenState.h"

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context) :
	Rinigin::State(context),
	m_GameMode(GameMode::SOLO),
	m_CurrentLevelIndex(0),
	m_Level(std::make_unique<Level>(Rinigin::SceneManager::GetInstance().GetActiveScene()))
{
	// Gamepads
	Rinigin::Gamepad* playerOneGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard

	// Skip button
	m_SkipLevelCommand = Rinigin::InputManager::GetInstance().AddCommand<GameCommands::SkipLevelCommand>(this);
	playerOneGamepad->AddBinding(SDL_SCANCODE_F2, Rinigin::BindingConnection::Down, m_SkipLevelCommand);

	//Rinigin::Gamepad* playerTwoGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(1) };

	m_Level->GetLevelObject()->SetActive(false);
	// TODO : Create characters
	m_DiggerOne = std::make_unique<DiggerMobile>(0,m_Level.get());	
	m_DiggerOne->GetRigidbody()->GravityEnabled(false);
	m_Level->AddPlayer(m_DiggerOne.get());


	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_DiggerOne->UpDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_DiggerOne->DownDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Down, m_DiggerOne->LeftDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Down, m_DiggerOne->RightDirectionCommand());

	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());

	// TEMPORARY WAY OF DAMAGING YOUR OWN CHARACTER
	playerOneGamepad->AddBinding(SDL_SCANCODE_F, Rinigin::BindingConnection::Down, m_DiggerOne->GetDamageCommand());

	m_DiggerTwo = std::make_unique<DiggerMobile>(1, m_Level.get());
	m_Level->AddPlayer(m_DiggerTwo.get());

	m_Nobbin = m_Level->GetEnemySpawner().Spawn("Nobbin");
} 

void GameScreenState::Enter()
{
	SetLevel(1);
	m_Level->SetScore(0);
	m_Level->Lives()->SetLives(DIGGER::DIGGER_LIVES);
	m_Level->GetLevelObject()->SetActive(true);
}

Rinigin::State* GameScreenState::Update()
{
	// GAME OVER CONDITION
	if (m_Level->Lives()->GetLives() <= 0) {
		if (m_Level->GetDeadPlayers().size() >= m_Level->GetPlayerCount()) {
			// TO END SCREEN
			return GetContext()->GetState<ScoreRegisterScreenState>();
		}
	}

	// NEXT LEVEL CONDITION
	const bool emeraldsCleared{ m_Level->GetEmeraldSpawner().GetInstances().size() <= 0 };
	const bool canMoveLevels{ emeraldsCleared };

	if (canMoveLevels) {
		// Move on to next level
		bool success = NextLevel();
		if (not success) {

			return GetContext()->GetState<ScoreRegisterScreenState>();
		}
	}

	return nullptr;
}

void GameScreenState::Exit()
{
	m_Level->GetLevelObject()->SetActive(false);
	m_Level->CleanUpLevel();
	Reset();
	// TODO : Remove bindings
	std::cout << "Game ended !" << std::endl;
}

void GameScreenState::SetLevel(int levelIndex)
{
	m_CurrentLevelIndex = levelIndex;
	m_Level->LoadLevel(levelIndex);
	StartGame();
}

bool GameScreenState::NextLevel()
{
	if (m_CurrentLevelIndex < DIGGER::MAX_LEVELS)
	{
		SetLevel(m_CurrentLevelIndex + 1);
		return true;
	}

	return false;
}

void GameScreenState::StartGame()
{
	Reset();
	switch (m_GameMode)
	{
	case GameMode::SOLO:
		StartSolo();
		break;
	case GameMode::COOP:
		StartCoop();
		break;
	case GameMode::PVP:
		// TODO : Spawn 1 player digger and 1 player enemy
		break;
	default:
		break;
	}
}

void GameScreenState::Reset()
{
	m_DiggerOne->GetCharacterObject()->SetActive(false); 
	m_DiggerTwo->GetCharacterObject()->SetActive(false);
	//m_Nobbin->GetCharacterObject()->SetActive(false);
}

void GameScreenState::StartSolo()
{
	m_Level->SetPlayerCount(1);
	m_Level->RespawnPlayer(0,false);
	std::cout << "Solo game!" << std::endl;
}

void GameScreenState::StartCoop()
{
	m_Level->SetPlayerCount(2);
	m_Level->RespawnPlayer(0,false);
	m_Level->RespawnPlayer(1,false);
}
