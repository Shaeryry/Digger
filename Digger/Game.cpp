#include "Game.h"

#include "InGameState.h"
#include "MainMenuState.h"

#include "SceneManager.h"

Game::Game() :
	m_MainMenuScene( std::make_unique<MainMenuState>(this) ),
	m_GameScene( std::make_unique<InGameState>(this) )
{
	LoadScene(m_MainMenuScene.get()); // Set the main menu as the default one
}

void Game::LoadScene(GameStateScene* gameStateScene)
{
	if (gameStateScene) {
		gameStateScene->InitializeScene();
		Rinigin::SceneManager::GetInstance().SetActiveScene(gameStateScene->GetScene());
	}
}
