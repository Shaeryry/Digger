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
#include "ScoreRegisterScreenState.h"

#include "Minigin.h"
#include "Gamepad.h"
#include "InputManager.h"
#include "MenuCommand.h"


DiggerGame::DiggerGame(float screenWidth, float screenHeight) :
	m_ScreenWidth(static_cast<int>(screenWidth)),
	m_ScreenHeight(static_cast<int>(screenHeight)),
	m_PlayerOne(nullptr),
	m_PlayerTwo(nullptr),
	m_GameScene(nullptr),

	m_MenuMoveUpCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectUp")),
	m_MenuMoveDownCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectDown")),
	m_MenuMoveLeftCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectLeft")),
	m_MenuMoveRightCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectRight")),
	m_MenuConfirmCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("Confirm"))
{
	// Initialize engine !
	Rinigin::Minigin engine("../Data/",m_ScreenWidth,m_ScreenHeight);
	engine.SetWindowTitle("DIGGER");
	engine.Run( std::bind(&DiggerGame::InitializeGame, this) ); // Run the engine !
}

void DiggerGame::InitializeGame()
{
	// Physics
	Rinigin::Physics::GetInstance().SetGravity(100.f);

	// Input
	m_PlayerOne = Rinigin::InputManager::GetInstance().GetGamepad(0); // Set player 1 to the keyboard
	m_PlayerTwo = Rinigin::InputManager::GetInstance().GetGamepad(1); // First controller that is found
	RegisterBindings();

	// Register Services
	Rinigin::ServiceLocator::RegisterSoundSystem(std::make_unique<Rinigin::SDLMixerSoundService>("../Data/Sounds/"));

	// Scene
	m_GameScene = Rinigin::SceneManager::GetInstance().CreateScene("Digger");
	Rinigin::SceneManager::GetInstance().SetActiveScene(m_GameScene); // Set the game's scene !
	Rinigin::GameObject* menuStateObject = m_GameScene->CreateObject();

	// Create a state context component

	Rinigin::StateContextComponent* gameContext = menuStateObject->AddComponent<Rinigin::StateContextComponent>();
	GameScreenState* gameState = gameContext->CreateState<GameScreenState>();
	ScoreRegisterScreenState* scoreRegisterState = gameContext->CreateState<ScoreRegisterScreenState>(gameState);
	StartScreenState* startScreenState = gameContext->CreateState<StartScreenState>(m_PlayerOne); // Make it so player one controls the menu !

	m_MenuMoveUpCommand->GetInputEvent()->AddObserver(startScreenState);
	m_MenuMoveDownCommand->GetInputEvent()->AddObserver(startScreenState);
	m_MenuMoveLeftCommand->GetInputEvent()->AddObserver(startScreenState);
	m_MenuMoveRightCommand->GetInputEvent()->AddObserver(startScreenState);
	m_MenuConfirmCommand->GetInputEvent()->AddObserver(startScreenState);

	m_MenuMoveUpCommand->GetInputEvent()->AddObserver(scoreRegisterState);
	m_MenuMoveDownCommand->GetInputEvent()->AddObserver(scoreRegisterState);
	m_MenuMoveLeftCommand->GetInputEvent()->AddObserver(scoreRegisterState);
	m_MenuMoveRightCommand->GetInputEvent()->AddObserver(scoreRegisterState);
	m_MenuConfirmCommand->GetInputEvent()->AddObserver(scoreRegisterState);

	gameContext->SetState(startScreenState); // Set the first state to the start screen

}

void DiggerGame::RegisterBindings()
{
	m_SelectUpKeyboard = m_PlayerOne->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_MenuMoveUpCommand);
	m_SelectDownKeyboard = m_PlayerOne->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_MenuMoveDownCommand);
	m_SelectLeftKeyboard = m_PlayerOne->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Down, m_MenuMoveLeftCommand);
	m_SelectRightKeyboard = m_PlayerOne->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Down, m_MenuMoveRightCommand);
	m_ConfirmKeyboard = m_PlayerOne->AddBinding(SDL_SCANCODE_SPACE, Rinigin::BindingConnection::Down, m_MenuConfirmCommand);

	m_SelectUpGamepad = m_PlayerTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::Down, m_MenuMoveUpCommand);
	m_SelectDownGamepad = m_PlayerTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::Down, m_MenuMoveDownCommand);
	m_SelectLeftGamepad = m_PlayerTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::Down, m_MenuMoveLeftCommand);
	m_SelectRightGamepad = m_PlayerTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::Down, m_MenuMoveRightCommand);
	m_ConfirmGamepad = m_PlayerTwo->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_A, Rinigin::BindingConnection::Down, m_MenuConfirmCommand);
}
