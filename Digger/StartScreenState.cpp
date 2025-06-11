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

#include "GameScreenState.h"

#include "DiggerConstants.h"
#include "StateContextComponent.h"

StartScreenState::StartScreenState(Rinigin::StateContextComponent* context, Rinigin::Gamepad* gamepad) :
	Rinigin::State(context),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_MenuMoveUpCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectUp")),
	m_MenuMoveDownCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("SelectDown")),
	m_MenuConfirmCommand(Rinigin::InputManager::GetInstance().AddCommand<MenuCommand>("Confirm")),

	m_ScreenGameObject(nullptr),
	m_BackgroundTextureComponent(nullptr),
	m_ScreenRenderer(nullptr),

	m_TitleGameObject(nullptr),
	m_TitleWordComponent(nullptr),

	m_SelectUp(nullptr),
	m_SelectDown(nullptr),
	m_Confirm(nullptr),

	m_MenuController(gamepad)
{
	m_MenuMoveUpCommand->GetInputEvent()->AddObserver(this);
	m_MenuMoveDownCommand->GetInputEvent()->AddObserver(this);
	m_MenuConfirmCommand->GetInputEvent()->AddObserver(this);

	// Setup game object
	m_GameScreenState = dynamic_cast<GameScreenState*>(GetContext()->GetState<GameScreenState>());

	// Background
	m_ScreenGameObject = m_Scene->CreateObject();
	m_ScreenGameObject->SetPosition(0, 0, 0);

	m_ScreenRenderer = m_ScreenGameObject->AddComponent<Rinigin::TextureRendererComponent>();
	m_BackgroundTextureComponent = m_ScreenGameObject->AddComponent<Rinigin::TextureComponent>(m_ScreenRenderer,"VTITLE.BMP");
	m_BackgroundTextureComponent->SetTexture("VTITLE.BMP");

	// Title
	m_TitleGameObject = m_Scene->CreateObject();
	m_TitleGameObject->SetParent(m_ScreenGameObject);

	m_TitleWordComponent = m_TitleGameObject->AddComponent<LetterTextComponent>("DIGGER");
	m_TitleWordComponent->SetText("DIGGER");
	m_TitleWordComponent->SetLetterSpacing(15.f);

	m_TitleGameObject->SetPosition((DIGGER::SCREEN_WIDTH / 2.f) - (m_TitleWordComponent->GetLength() / 2), 5.f, 0);


	// Mode select
	m_ModeSelectionGameObject = m_Scene->CreateObject();
	m_ModeSelectionGameObject->SetParent(m_ScreenGameObject);

	m_ModeSelectWordComponent = m_ModeSelectionGameObject->AddComponent<LetterTextComponent>("DIGGER");
	m_ModeSelectWordComponent->SetText("GAMEMODE");
}  

void StartScreenState::Enter() 
{
    m_SelectUp = m_MenuController->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_MenuMoveUpCommand);
	m_SelectDown = m_MenuController->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_MenuMoveDownCommand);
	m_Confirm = m_MenuController->AddBinding(SDL_SCANCODE_SPACE, Rinigin::BindingConnection::Down, m_MenuConfirmCommand);

	//m_KeyboardUp = ->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommand);
	m_ScreenGameObject->SetActive(true);
	std::cout << "entered start screen" << std::endl;
	SetSelection(0);
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

	m_ScreenGameObject->SetActive(false);
	std::cout << "left start screen" << std::endl;
}

void StartScreenState::Notify(Rinigin::EventArguments & eventArguments)
{
	const int currentGameMode = static_cast<int>(m_GameScreenState->GetGameMode());

	switch (eventArguments.GetID())
	{
		case Rinigin::Helpers::sdbm_hash("SelectUp"):
			SetSelection(currentGameMode + 1);
			break;
		case Rinigin::Helpers::sdbm_hash("SelectDown"):
			SetSelection(currentGameMode - 1);
			break;
		case Rinigin::Helpers::sdbm_hash("Confirm"):
			GetContext()->SetState(m_GameScreenState);
			break;
		default:
			break;
	}
}

int StartScreenState::GetValidSelection(int index)
{
	return std::min(std::max(index,0),static_cast<int>(GameMode::END)-1);
}

void StartScreenState::SetSelection(int modeIndex)
{
	if (m_GameScreenState) {
		const int selectionIndex = GetValidSelection(modeIndex);
		m_GameScreenState->SetGameMode(static_cast<GameMode>(selectionIndex));
		m_ModeSelectWordComponent->SetText(m_GAMEMODE_NAMES[selectionIndex]);
		m_ModeSelectionGameObject->SetPosition((DIGGER::SCREEN_WIDTH * .75f) - (m_ModeSelectWordComponent->GetLength() / 2), 50.f, 0);
	}
}
