#pragma once
#include <State.h>
#include "Observer.h"

namespace Rinigin { class GameObject; }
class LetterTextComponent;
class GameScreenState;
class MenuCommand;

class ScoreRegisterScreenState : public Rinigin::State,public Rinigin::Observer
{
public:
	explicit ScoreRegisterScreenState(Rinigin::StateContextComponent* context,GameScreenState* gameState);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	void Notify(Rinigin::EventArguments& arguments) override;
private:
	void UpdateInitialsField();
	void ShiftPosition(int index);
	void ShiftLetter(int index);

	bool m_Saving;
	bool m_Saved;
	int m_SelectedLetter;
	std::string m_Abbreviation;

	GameScreenState* m_GameState;
	Rinigin::GameObject* m_ScoreScreenGameObject;

	// Appearance
	Rinigin::GameObject* m_ScoreTextGameObject;
	Rinigin::GameObject* m_TitleGameObject;
	Rinigin::GameObject* m_HighScoreTextGameObject;
	Rinigin::GameObject* m_EnterInitialTextGameObject;
	Rinigin::GameObject* m_InitialsGameObject;

	LetterTextComponent* m_ScoreTextComponent;
	LetterTextComponent* m_TitleTextComponent;
	LetterTextComponent* m_HighScoreTextComponent;
	LetterTextComponent* m_InitialsTextComponent;
	LetterTextComponent* m_InitialsEnterTextComponent;

};

