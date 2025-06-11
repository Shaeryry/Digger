#include "DiggerGame.h"

#include "SDLMixerSoundSystem.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PhysicsManager.h"

#include "StateContextComponent.h" 
#include "StartScreenState.h"
#include "GameScreenState.h"

#include "Minigin.h"


DiggerGame::DiggerGame(float screenWidth, float screenHeight) :
	m_ScreenWidth(static_cast<int>(screenWidth)),
	m_ScreenHeight(static_cast<int>(screenHeight)),
	m_PlayerOne(nullptr),
	m_PlayerTwo(nullptr),
	m_GameScene(nullptr)
{
	// Initialize engine !
	Rinigin::Minigin engine("../Data/",m_ScreenWidth,m_ScreenHeight);
	engine.SetWindowTitle("DIGGER");
	engine.Run( std::bind(&DiggerGame::InitializeGame, this) ); // Run the engine !
}

void DiggerGame::InitializeGame()
{
	Rinigin::Physics::GetInstance().SetGravity(100.f);

	m_GameScene = Rinigin::SceneManager::GetInstance().CreateScene("Digger");
	m_PlayerOne = Rinigin::InputManager::GetInstance().GetGamepad(0); // Set player 1 to the keyboard
	m_PlayerTwo = Rinigin::InputManager::GetInstance().GetGamepad(1); // First controller that is found

	Rinigin::ServiceLocator::RegisterSoundSystem(std::make_unique<Rinigin::SDLMixerSoundService>("../Data/Sounds/"));

	Rinigin::SceneManager::GetInstance().SetActiveScene(m_GameScene); // Set the game's scene !
	Rinigin::GameObject* menuStateObject = m_GameScene->CreateObject();

	// Create a state context component
	Rinigin::StateContextComponent* gameContext = menuStateObject->AddComponent<Rinigin::StateContextComponent>();
	gameContext->CreateState<GameScreenState>();
	StartScreenState* startScreenState = gameContext->CreateState<StartScreenState>(m_PlayerOne); // Make it so player one controls the menu !

	gameContext->SetState(startScreenState); // Set the first state to the start screen
	/*gameState->SetGameMode(GameMode::COOP);
	gameContext->SetState(gameState);*/

}
