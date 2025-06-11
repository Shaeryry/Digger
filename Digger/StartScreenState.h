#pragma once
#include "Observer.h"
#include "State.h"
#include "InputManager.h"

namespace Rinigin { class Scene; class Gamepad; class Command; class TextureComponent; class TextureRendererComponent; }
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

	//HIGHSCORE TEXT
	Rinigin::GameObject* m_HighscoreGameObject;
	LetterTextComponent* m_HighscoreTextComponent;

	// Mode Display
	Rinigin::GameObject* m_ModeSelectionGameObject;
	LetterTextComponent* m_ModeSelectWordComponent;

	std::vector<Rinigin::GameObject*> m_HighscoreScoreGameObjects;
	std::vector<Rinigin::GameObject*> m_HighscoreInitialsGameObjects;
	std::vector<LetterTextComponent*> m_HighscoreInitialTextComponents;
	std::vector<LetterTextComponent*> m_HighscoreScoreTextComponents;
};

