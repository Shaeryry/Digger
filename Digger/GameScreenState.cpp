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

#include "ServiceLocator.h"
#include "Timer.h"

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context) :
	Rinigin::State(context),
	m_GameMode(GameMode::SOLO),
	m_CurrentLevelIndex(0),
	m_AllPlayersDead(false),
	m_Level(std::make_unique<Level>(Rinigin::SceneManager::GetInstance().GetActiveScene()))
{
	m_Level->GetLevelObject()->SetActive(false);
	m_SkipLevelCommand = Rinigin::InputManager::GetInstance().AddCommand<GameCommands::SkipLevelCommand>(this);
	// TODO : Create characters
	m_DiggerOne = std::make_unique<DiggerMobile>(0,m_Level.get());	
	m_DiggerOne->GetRigidbody()->GravityEnabled(false);
	m_Level->AddPlayer(m_DiggerOne.get());

	m_DiggerTwo = std::make_unique<DiggerMobile>(1, m_Level.get());
	m_Level->AddPlayer(m_DiggerTwo.get());

	m_Nobbin = m_Level->GetEnemySpawner().Spawn("Nobbin");
} 

void GameScreenState::Enter()
{
	SetupBindings();

	SetLevel(1);
	m_Level->SetScore(0);
	m_Level->Lives()->SetLives(DIGGER::DIGGER_LIVES);
	m_Level->GetLevelObject()->SetActive(true);
}

Rinigin::State* GameScreenState::Update()
{
	const bool allPlayersCurrentlyDead = m_Level->GetDeadPlayers().size() >= m_Level->GetPlayerCount();

	// Already in transition? Count down
	if (m_TransitioningToNextLevel || m_TransitioningToScore)
	{
		m_TransitionTimer += Rinigin::Timer::GetInstance().deltaTime;
		if (m_TransitionTimer > DIGGER::TRANSITION_TIME)
		{
			if (m_TransitioningToScore)
			{
				return GetContext()->GetState<ScoreRegisterScreenState>();
			}
			else if (m_TransitioningToNextLevel)
			{
				bool success = NextLevel();
				if (!success)
				{
					return GetContext()->GetState<ScoreRegisterScreenState>();
				}
				m_TransitioningToNextLevel = false;
			} 
		}
		else {
			Reset();
		}
		return nullptr;
	}

	// Game over
	if (m_Level->Lives()->GetLives() <= 0 && allPlayersCurrentlyDead)
	{
		m_TransitioningToScore = true;
		m_TransitionTimer = 0;
		return nullptr;
	}

	// All emeralds collected
	const bool emeraldsCleared{ m_Level->GetEmeraldSpawner().GetInstances().size() <= 0 };
	if (emeraldsCleared)
	{
		Rinigin::ServiceLocator::GetSoundService().Play({ "NextLevel.wav",0.5f,true });
		m_TransitioningToNextLevel = true;
		m_TransitionTimer = 0;
		return nullptr;
	}

	// Death music / respawn
	if (allPlayersCurrentlyDead != m_AllPlayersDead)
	{
		if (allPlayersCurrentlyDead)
			Rinigin::ServiceLocator::GetSoundService().Play({ "DeathTrack.wav",0.5f,true });
		else
			Rinigin::ServiceLocator::GetSoundService().Play({ "MainMusic.wav",0.5f,true });
	}
	m_AllPlayersDead = allPlayersCurrentlyDead;

	return nullptr;
}


//Rinigin::State* GameScreenState::Update()
//{
//	const bool allPlayersCurrentlyDead = m_Level->GetDeadPlayers().size() >= m_Level->GetPlayerCount();
//
//	// GAME OVER CONDITION
//	if (m_Level->Lives()->GetLives() <= 0) {
//		if (allPlayersCurrentlyDead) {
//			// TO END SCREEN
//			return GetContext()->GetState<ScoreRegisterScreenState>();
//		}
//	}
//
//	// NEXT LEVEL CONDITION
//	const bool emeraldsCleared{ m_Level->GetEmeraldSpawner().GetInstances().size() <= 0 };
//	const bool canMoveLevels{ emeraldsCleared };
//
//	if (canMoveLevels) {
//		// Move on to next level
//		bool success = NextLevel();
//		if (not success) {
//
//			return GetContext()->GetState<ScoreRegisterScreenState>();
//		}
//	}
//
//	// PLAYERS JUST ALL DIED
//	if (allPlayersCurrentlyDead != m_AllPlayersDead) {
//		if (allPlayersCurrentlyDead) {
//			Rinigin::ServiceLocator::GetSoundService().Play({ "DeathTrack.wav",0.5f,true }); // Play death music
//		}
//		else {
//			Rinigin::ServiceLocator::GetSoundService().Play({ "MainMusic.wav",0.5f,true }); // Play music
//		}
//	}
//	m_AllPlayersDead = allPlayersCurrentlyDead;
//
//
//	return nullptr;
//}

void GameScreenState::Exit()
{
	RemoveBindings();

	m_Level->GetLevelObject()->SetActive(false);
	m_Level->CleanUpLevel();

	Rinigin::ServiceLocator::GetSoundService().Stop({ "MainMusic.wav" }); // stop music
	Reset();

	std::cout << "Game ended !" << std::endl;
}

void GameScreenState::SetLevel(int levelIndex)
{
	m_CurrentLevelIndex = levelIndex;
	m_Level->LoadLevel(levelIndex);
	Rinigin::ServiceLocator::GetSoundService().Play({ "MainMusic.wav",0.5f,true }); // Play music
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

void GameScreenState::SetupBindings()
{
	// Gamepads
	Rinigin::Gamepad* playerKeyboard{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard
	Rinigin::Gamepad* gamepadOne{ Rinigin::InputManager::GetInstance().GetGamepad(1) }; // Gamepad 1
	Rinigin::Gamepad* gamepadTwo{ Rinigin::InputManager::GetInstance().GetGamepad(2) }; // Gamepad 2

	// Bindings

	// Keyboard
		m_KeyboardSkipLevel = playerKeyboard->AddBinding(SDL_SCANCODE_F1, Rinigin::BindingConnection::Down, m_SkipLevelCommand); // Skip level

		// Direction
		m_KeyboardDirectionUp = playerKeyboard->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_DiggerOne->UpDirectionCommand());
		m_KeyboardDirectionDown = playerKeyboard->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_DiggerOne->DownDirectionCommand());
		m_KeyboardDirectionLeft = playerKeyboard->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Down, m_DiggerOne->LeftDirectionCommand());
		m_KeyboardDirectionRight = playerKeyboard->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Down, m_DiggerOne->RightDirectionCommand());

		// Movement
		m_KeyboardMoveUp = playerKeyboard->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_KeyboardMoveDown = playerKeyboard->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_KeyboardMoveLeft = playerKeyboard->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_KeyboardMoveRight = playerKeyboard->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());

	// Gamepads

	// 1

		// Direction
		m_GamepadOneDirectionUp = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::Down, m_DiggerOne->UpDirectionCommand());
		m_GamepadOneDirectionDown = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::Down, m_DiggerOne->DownDirectionCommand());
		m_GamepadOneDirectionLeft = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::Down, m_DiggerOne->LeftDirectionCommand());
		m_GamepadOneDirectionRight = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::Down, m_DiggerOne->RightDirectionCommand());

		// Movement
		m_GamepadOneMoveUp = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_GamepadOneMoveDown = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_GamepadOneMoveLeft = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
		m_GamepadOneMoveRight = gamepadOne->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());

	// 2

		// Direction
		m_GamepadTwoDirectionUp = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::Down, m_DiggerTwo->UpDirectionCommand());
		m_GamepadTwoDirectionDown = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::Down, m_DiggerTwo->DownDirectionCommand());
		m_GamepadTwoDirectionLeft = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::Down, m_DiggerTwo->LeftDirectionCommand());
		m_GamepadTwoDirectionRight = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::Down, m_DiggerTwo->RightDirectionCommand());
				 
		// Movement
		m_GamepadTwoMoveUp = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::Held, m_DiggerTwo->GetMoveCommand());
		m_GamepadTwoMoveDown = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::Held, m_DiggerTwo->GetMoveCommand());
		m_GamepadTwoMoveLeft = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::Held, m_DiggerTwo->GetMoveCommand());
		m_GamepadTwoMoveRight = gamepadTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::Held, m_DiggerTwo->GetMoveCommand());

}

void GameScreenState::RemoveBindings()
{
	Rinigin::Gamepad* playerKeyboard{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard
	Rinigin::Gamepad* gamepadOne{ Rinigin::InputManager::GetInstance().GetGamepad(1) };
	Rinigin::Gamepad* gamepadTwo{ Rinigin::InputManager::GetInstance().GetGamepad(2) }; 

	// Unbind keyboard
		playerKeyboard->RemoveBinding(m_KeyboardSkipLevel);

		playerKeyboard->RemoveBinding(m_KeyboardDirectionUp);
		playerKeyboard->RemoveBinding(m_KeyboardDirectionDown);
		playerKeyboard->RemoveBinding(m_KeyboardDirectionLeft);
		playerKeyboard->RemoveBinding(m_KeyboardDirectionRight);

		playerKeyboard->RemoveBinding(m_KeyboardMoveUp);
		playerKeyboard->RemoveBinding(m_KeyboardMoveDown);
		playerKeyboard->RemoveBinding(m_KeyboardMoveLeft);
		playerKeyboard->RemoveBinding(m_KeyboardMoveRight);

	// Unbind gamepad one
		gamepadOne->RemoveBinding(m_GamepadOneDirectionUp);
		gamepadOne->RemoveBinding(m_GamepadOneDirectionDown);
		gamepadOne->RemoveBinding(m_GamepadOneDirectionLeft);
		gamepadOne->RemoveBinding(m_GamepadOneDirectionRight);

		gamepadOne->RemoveBinding(m_GamepadOneMoveUp);
		gamepadOne->RemoveBinding(m_GamepadOneMoveDown);
		gamepadOne->RemoveBinding(m_GamepadOneMoveLeft);
		gamepadOne->RemoveBinding(m_GamepadOneMoveRight);

	// Unbind gamepad two
		gamepadTwo->RemoveBinding(m_GamepadTwoDirectionUp);
		gamepadTwo->RemoveBinding(m_GamepadTwoDirectionDown);
		gamepadTwo->RemoveBinding(m_GamepadTwoDirectionLeft);
		gamepadTwo->RemoveBinding(m_GamepadTwoDirectionRight);

		gamepadTwo->RemoveBinding(m_GamepadTwoMoveUp);
		gamepadTwo->RemoveBinding(m_GamepadTwoMoveDown);
		gamepadTwo->RemoveBinding(m_GamepadTwoMoveLeft);
		gamepadTwo->RemoveBinding(m_GamepadTwoMoveRight);
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
