#include "GameScreenState.h"
#include <iostream>
#include "SceneManager.h"

#include "GameObject.h"
#include "Character.h"
#include "Digger.h"

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context) :
	Rinigin::State(context),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_GameMode(GameMode::SOLO)
{
	// TODO : Create characters
	m_DiggerOne = dynamic_cast<Digger*>(AddCharacter(new Digger()));
	m_DiggerTwo = dynamic_cast<Digger*>(AddCharacter(new Digger()));
}

void GameScreenState::Enter()
{
	std::cout << "Game started !" << std::endl;
	// TODO : Generate map
	Reset();

	switch (m_GameMode)
	{
		case GameMode::SOLO:
			StartSolo();
			// TODO : Spawn 1 players
			break;
		case GameMode::COOP:
			// TODO : Spawn 2 players
			break;
		case GameMode::PVP:
			// TODO : Spawn 1 player digger and 1 player enemy
			break;
	default:
		break;
	}
}

void GameScreenState::Update()
{
}

void GameScreenState::Exit()
{
	Reset();
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
	std::cout << "Solo game!" << std::endl;
}

Character* GameScreenState::AddCharacter(Character* character)
{ 
	std::unique_ptr<Character> characterPointer(character);
	m_Characters.emplace_back( std::move(characterPointer) );
	return m_Characters.back().get();
}