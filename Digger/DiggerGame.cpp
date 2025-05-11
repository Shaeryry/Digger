#include "DiggerGame.h"

#include "SDLMixerSoundSystem.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "StateContextComponent.h" 
#include "StartScreenState.h"
#include "GameScreenState.h"

DiggerGame::DiggerGame() :
	m_GameScene( Rinigin::SceneManager::GetInstance().CreateScene("Digger") ),
	m_PlayerOne( Rinigin::InputManager::GetInstance().GetGamepad(0) ), // Set player 1 to the keyboard
	m_PlayerTwo( Rinigin::InputManager::GetInstance().GetGamepad(1) ) // First controller that is found
{
	Rinigin::ServiceLocator::RegisterSoundSystem( std::make_unique<Rinigin::SDLMixerSoundService>("../Data/Sounds/") );

	Rinigin::SceneManager::GetInstance().SetActiveScene(m_GameScene); // Set the game's scene !
	Rinigin::GameObject* menuStateObject = m_GameScene->CreateObject();

	// Create a state context component
	Rinigin::StateContextComponent* gameContext = menuStateObject->AddComponent<Rinigin::StateContextComponent>();
	StartScreenState* startScreenState = gameContext->CreateState<StartScreenState>(m_PlayerOne); // Make it so player one controls the menu !
	GameScreenState* gameState = gameContext->CreateState<GameScreenState>();

	gameContext->SetState(startScreenState); // Set the first state to the start screen
	gameContext->SetState(gameState);
}