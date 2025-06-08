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

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context) :
	Rinigin::State(context),
	m_GameMode(GameMode::SOLO),
	m_Level(std::make_unique<Level>(Rinigin::SceneManager::GetInstance().GetActiveScene()))
{

	Rinigin::Gamepad* playerOneGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard
	//Rinigin::Gamepad* playerTwoGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(1) };

	// TODO : Create characters
	m_DiggerOne = std::make_unique<DiggerMobile>(0,m_Level->Map());
	m_DiggerOne->GetRigidbody()->GravityEnabled(false);

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

	m_DiggerTwo = std::make_unique<DiggerMobile>(0, m_Level->Map());
	//m_DiggerTwo->GetCharacterObject()->SetPosition(screenSize.x / 2, screenSize.y / 2, 0);

	// TEMPORARY DISPLAY
} 


void GameScreenState::Enter()
{
	Reset();
	m_Level->LoadLevel(1);
	// TODO : Generate level

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

Rinigin::State* GameScreenState::Update()
{
	return nullptr;
}

void GameScreenState::Exit()
{
	Reset();
	// TODO : Remove bindings
	std::cout << "Game ended !" << std::endl;
}

void GameScreenState::Reset()
{
	m_DiggerOne->GetCharacterObject()->SetActive(false); 
	m_DiggerTwo->GetCharacterObject()->SetActive(false);
}

void GameScreenState::StartSolo()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	m_DiggerOne->GetCharacterObject()->SetPosition( m_Level->GetPlayerSpawnIndex(0) );
	std::cout << "Solo game!" << std::endl;
}

void GameScreenState::StartCoop()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	m_DiggerOne->GetCharacterObject()->SetPosition(m_Level->GetPlayerSpawnIndex(0));

	m_DiggerTwo->GetCharacterObject()->SetActive(true);
	m_DiggerTwo->GetCharacterObject()->SetPosition(m_Level->GetPlayerSpawnIndex(1));

}
