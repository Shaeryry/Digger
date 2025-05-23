#include "GameScreenState.h"
#include <iostream>
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

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context, DiggerGame* game) :
	Rinigin::State(context),
	m_Game(game),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_GameMode(GameMode::SOLO),
	m_MapComponent(nullptr)
{
	glm::vec2 screenSize = m_Game->GetScreenSize();

	m_BackgroundGameObject = m_Scene->CreateObject();
	m_MapComponent = m_BackgroundGameObject->AddComponent<TerrainComponent>(screenSize,screenSize,40);

	Rinigin::Gamepad* playerOneGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard
	//Rinigin::Gamepad* playerTwoGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(1) };

	// TODO : Create characters
	m_DiggerOne = dynamic_cast<DiggerMobile*>(AddCharacter( new DiggerMobile(0, m_MapComponent) ));
	m_DiggerOne->GetCharacterObject()->SetPosition(screenSize.x / 2, screenSize.y / 2, 0);
	m_DiggerOne->GetRigidbody()->GravityEnabled(false);

	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::OnTrigger, m_DiggerOne->UpDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::OnTrigger, m_DiggerOne->DownDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::OnTrigger, m_DiggerOne->LeftDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::OnTrigger, m_DiggerOne->RightDirectionCommand());

	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::OnHeld, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::OnHeld, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::OnHeld, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::OnHeld, m_DiggerOne->GetMoveCommand());

	// TEMPORARY WAY OF DAMAGING YOUR OWN CHARACTER
	playerOneGamepad->AddBinding(SDL_SCANCODE_F, Rinigin::BindingConnection::OnTrigger, m_DiggerOne->GetDamageCommand());

	m_DiggerTwo = dynamic_cast<DiggerMobile*>(AddCharacter(new DiggerMobile(1,m_MapComponent)));
	m_DiggerTwo->GetCharacterObject()->SetPosition(screenSize.x / 2, screenSize.y / 2, 0);

	m_MapComponent->DigAt(screenSize.x / 2, screenSize.y / 2, 100);

	// TEMPORARY DISPLAY
} 


void GameScreenState::Enter()
{
	Reset();
	LoadLevel();
	// TODO : Generate level

	switch (m_GameMode)
	{
		case GameMode::SOLO:
			StartSolo();
			// TODO : Spawn 1 players
			break;
		case GameMode::COOP:
			// TODO : Spawn 2 players
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

void GameScreenState::LoadLevel()
{
	std::cout << "level loader" << std::endl;
	auto* font = Rinigin::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	 
	auto* textGO = m_Scene->CreateObject();
	auto* textRenderer = textGO->AddComponent<Rinigin::TextureRendererComponent>();
	auto* text = textGO->AddComponent<Rinigin::TextComponent>(textRenderer, "Press 'F' to kill the digger mobile.", font);
	text->SetColor(255, 255, 255);
	textGO->SetActive(true);
	textGO->SetPosition(0, 0, 0);

	// MAP SETTING

	m_MapComponent->ChangeBackgroundTexture("../Data/MapTextures/Level3.png");
}

void GameScreenState::StartSolo()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	std::cout << "Solo game!" << std::endl;
}

void GameScreenState::StartCoop()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	m_DiggerTwo->GetCharacterObject()->SetActive(true);
}

Character* GameScreenState::AddCharacter(Character* character)
{ 
	std::unique_ptr<Character> characterPointer(character);
	m_Characters.emplace_back( std::move(characterPointer) );
	return m_Characters.back().get();
}