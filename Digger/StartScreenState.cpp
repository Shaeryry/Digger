#include "StartScreenState.h"
#include <iostream>
#include "SceneManager.h"
#include "StartScreenMenuCommand.h"
#include "Gamepad.h"
#include "Helpers.h"
#include "Scene.h"

#include "TextureComponent.h"
#include "TextureRendererComponent.h"
#include "LetterTextComponent.h"

StartScreenState::StartScreenState(Rinigin::StateContextComponent* context, Rinigin::Gamepad* gamepad) :
	Rinigin::State(context),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_MenuMoveUpCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectUp")),
	m_MenuMoveDownCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectDown")),
	m_MenuConfirmCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("Confirm")),

	m_ScreenGameObject(nullptr),
	m_BackgroundTextureComponent(nullptr),
	m_ScreenRenderer(nullptr),

	m_SelectUp(nullptr),
	m_SelectDown(nullptr),
	m_Confirm(nullptr),

	m_MenuController(gamepad)
{
	m_MenuMoveUpCommand->GetInputEvent()->AddObserver(this);
	m_MenuMoveDownCommand->GetInputEvent()->AddObserver(this);
	m_MenuConfirmCommand->GetInputEvent()->AddObserver(this);

	// Setup game object
	m_ScreenGameObject = m_Scene->CreateObject();
	m_ScreenGameObject->SetPosition(0, 0, 0);

	m_ScreenRenderer = m_ScreenGameObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_BackgroundTextureComponent = m_ScreenGameObject->AddComponent<Rinigin::TextureComponent>(m_ScreenRenderer,"VTITLE.BMP");
	m_BackgroundTextureComponent->SetTexture("VTITLE.BMP");

	m_ScreenGameObject->AddComponent<LetterTextComponent>(".:_");
}  

void StartScreenState::Enter() 
{
    m_SelectUp = m_MenuController->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_MenuMoveUpCommand);
	m_SelectDown = m_MenuController->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_MenuMoveDownCommand);
	m_Confirm = m_MenuController->AddBinding(SDL_SCANCODE_SPACE, Rinigin::BindingConnection::Down, m_MenuConfirmCommand);

	//m_KeyboardUp = ->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommand);
	//m_ScreenGameObject->SetActive(true);
	std::cout << "entered start screen" << std::endl;
}

Rinigin::State* StartScreenState::Update()
{
	return nullptr;
}

void StartScreenState::Exit()
{
	m_MenuController->RemoveBinding(m_SelectUp);
	m_MenuController->RemoveBinding(m_SelectDown);
	m_MenuController->RemoveBinding(m_Confirm);

	//m_ScreenGameObject->SetActive(false);
	std::cout << "left start screen" << std::endl;
}

void StartScreenState::Notify(Rinigin::EventArguments & eventArguments)
{
	switch (eventArguments.GetID())
	{
		case Rinigin::Helpers::sdbm_hash("SelectUp"):
			std::cout << "Move up !" << std::endl;
			break;
		case Rinigin::Helpers::sdbm_hash("SelectDown"):
			std::cout << "Move down !" << std::endl;
			break;
		case Rinigin::Helpers::sdbm_hash("Confirm"):
			std::cout << "Confirm !" << std::endl;
			break;
		default:
			break;
	}
}
