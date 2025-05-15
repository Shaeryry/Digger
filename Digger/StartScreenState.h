#pragma once
#include "Observer.h"
#include "State.h"
#include "InputManager.h"

class MenuCommand;
namespace Rinigin { class Scene; class Gamepad; class Command; }

class StartScreenState final : public Rinigin::State,public Rinigin::Observer
{
public:
	explicit StartScreenState(Rinigin::StateContextComponent* context,Rinigin::Gamepad* gamepad); 

	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	virtual void Notify(Rinigin::EventArguments* arguments) override;
private:
	Rinigin::Scene* m_Scene;
	Rinigin::Gamepad* m_MenuController;
	
	// Commands
	MenuCommand* m_MenuMoveUpCommand;
	MenuCommand* m_MenuMoveDownCommand;
	MenuCommand* m_MenuConfirmCommand;

	// Bindings
	Rinigin::Binding* m_SelectUp;
	Rinigin::Binding* m_SelectDown;
	Rinigin::Binding* m_Confirm;
};

