#pragma once
#include "Observer.h"
#include "State.h"
#include "InputManager.h"

namespace Rinigin { class Scene; class Gamepad; class Command; class TextureComponent; class TextureRendererComponent; }
class MenuCommand;
class LetterTextComponent;
class GameScreenState;

class StartScreenState final : public Rinigin::State,public Rinigin::Observer
{
public:
	explicit StartScreenState(Rinigin::StateContextComponent* context,Rinigin::Gamepad* gamepad); 

	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	virtual void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	int GetValidSelection(int index);
	void SetSelection(int modeIndex);
	const char* m_GAMEMODE_NAMES[3]{"SOLO", "COOP", "VERSUS"};

	GameScreenState* m_GameScreenState;
	Rinigin::Scene* m_Scene;
	Rinigin::Gamepad* m_MenuController;
	
	// Background
	Rinigin::GameObject* m_ScreenGameObject;
	Rinigin::TextureRendererComponent* m_ScreenRenderer;
	Rinigin::TextureComponent* m_BackgroundTextureComponent;

	//Title
	Rinigin::GameObject* m_TitleGameObject;
	LetterTextComponent* m_TitleWordComponent;

	// Mode Display
	Rinigin::GameObject* m_ModeSelectionGameObject;
	LetterTextComponent* m_ModeSelectWordComponent;

	// Commands
	MenuCommand* m_MenuMoveUpCommand;
	MenuCommand* m_MenuMoveDownCommand;
	MenuCommand* m_MenuConfirmCommand;

	// Bindings
	Rinigin::Binding* m_SelectUp;
	Rinigin::Binding* m_SelectDown;
	Rinigin::Binding* m_Confirm;
};

