#include "StartScreenState.h"
#include <iostream>
#include "SceneManager.h"
#include "StartScreenMenuCommand.h"
#include "Gamepad.h"
#include "Helpers.h"

StartScreenState::StartScreenState(Rinigin::StateContextComponent* context,Rinigin::Gamepad* gamepad) :
	Rinigin::State(context),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_MenuMoveUpCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectUp")),
	m_MenuMoveDownCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectDown")),
	m_MenuConfirmCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("Confirm")),

	m_SelectUp(nullptr),
	m_SelectDown(nullptr),
	m_Confirm(nullptr),

	m_MenuController(gamepad)
{
	m_MenuMoveUpCommand->GetInputEvent()->AddObserver(this);
	m_MenuMoveDownCommand->GetInputEvent()->AddObserver(this);
	m_MenuConfirmCommand->GetInputEvent()->AddObserver(this);
}  

void StartScreenState::Enter() 
{
    m_SelectUp = m_MenuController->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::OnTrigger, m_MenuMoveUpCommand);
	m_SelectDown = m_MenuController->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::OnTrigger, m_MenuMoveDownCommand);
	m_Confirm = m_MenuController->AddBinding(SDL_SCANCODE_SPACE, Rinigin::BindingConnection::OnTrigger, m_MenuConfirmCommand);

	//m_KeyboardUp = ->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommand);
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

	std::cout << "left start screen" << std::endl;
}

void StartScreenState::Notify(Rinigin::EventArguments* arguments)
{
	switch (arguments->GetID())
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
