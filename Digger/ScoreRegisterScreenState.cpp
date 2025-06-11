#include "ScoreRegisterScreenState.h"

#include "SceneManager.h"
#include "Scene.h"
#include "LetterTextComponent.h"
#include "DiggerConstants.h"
#include "StateContextComponent.h"
#include "GameScreenState.h"
#include "Helpers.h"
#include "Level.h"	
#include "InputManager.h"
#include "MenuCommand.h"
#include "HighScoreManager.h"

#include <iostream>
#include "StartScreenState.h"
#include "StateContextComponent.h"

ScoreRegisterScreenState::ScoreRegisterScreenState(Rinigin::StateContextComponent* context, GameScreenState* gameState) :
	State(context),
	m_ScoreScreenGameObject(nullptr),
	m_GameState(gameState),
	m_SelectedLetter(0),
	m_Saving(false),
	m_Saved(false)
{
	Rinigin::Scene* scene = Rinigin::SceneManager::GetInstance().GetActiveScene();

	// GameObject
	m_ScoreScreenGameObject = scene->CreateObject();
	m_ScoreScreenGameObject->SetActive(false);

	// Title Text
	m_TitleGameObject = scene->CreateObject();
	m_TitleGameObject->SetParent(m_ScoreScreenGameObject);
	m_TitleTextComponent = m_TitleGameObject->AddComponent<LetterTextComponent>("PLAYER 1");
	m_TitleGameObject->SetPosition((DIGGER::SCREEN_WIDTH * .5f) - (m_TitleTextComponent->GetLength() / 2), 10, 0);

	// Score Text
	m_ScoreTextGameObject = scene->CreateObject();
	m_ScoreTextGameObject->SetParent(m_ScoreScreenGameObject);
	m_ScoreTextComponent = m_ScoreTextGameObject->AddComponent<LetterTextComponent>("SCORE");
	m_ScoreTextGameObject->SetPosition(10.f, 10.f, 0);


	// Highscore Text
	m_HighScoreTextGameObject = scene->CreateObject();
	m_HighScoreTextGameObject->SetParent(m_ScoreScreenGameObject);
	m_HighScoreTextComponent = m_HighScoreTextGameObject->AddComponent<LetterTextComponent>("NEW HIGHSCORE");
	m_HighScoreTextComponent->SetColor({255, 0, 0});
	m_HighScoreTextGameObject->SetPosition((DIGGER::SCREEN_WIDTH * .5f) - (m_HighScoreTextComponent->GetLength()/2), (DIGGER::SCREEN_HEIGHT * .3f), 0);

	// Initials Text
	m_EnterInitialTextGameObject = scene->CreateObject();
	m_EnterInitialTextGameObject->SetParent(m_ScoreScreenGameObject);
	m_InitialsTextComponent = m_EnterInitialTextGameObject->AddComponent<LetterTextComponent>("ENTER INITIALS");
	m_EnterInitialTextGameObject->SetPosition((DIGGER::SCREEN_WIDTH * .5f) - (m_InitialsTextComponent->GetLength()/2), (DIGGER::SCREEN_HEIGHT * .45f), 0);

	// Initials Text
	m_InitialsGameObject = scene->CreateObject();
	m_InitialsGameObject->SetParent(m_ScoreScreenGameObject);
	m_InitialsEnterTextComponent = m_InitialsGameObject->AddComponent<LetterTextComponent>("___");
	m_InitialsEnterTextComponent->SetLetterSpacing(15.f);
	m_InitialsGameObject->SetPosition((DIGGER::SCREEN_WIDTH * .5f) - (m_InitialsEnterTextComponent->GetLength() / 2), (DIGGER::SCREEN_HEIGHT * .55f), 0);

}

void ScoreRegisterScreenState::Enter()
{
	m_Saved = false;
	m_InitialsEnterTextComponent->SetText("___");
	m_ScoreTextComponent->SetText( Rinigin::Helpers::GetFormattedScore(m_GameState->GetLevel()->Score()).c_str() );
	m_ScoreScreenGameObject->SetActive(true);
}

Rinigin::State* ScoreRegisterScreenState::Update()
{
	if (m_Saved) {
		return GetContext()->GetState<StartScreenState>();
	}
	return nullptr;
}

void ScoreRegisterScreenState::Exit()
{
	m_ScoreScreenGameObject->SetActive(false);
}

void ScoreRegisterScreenState::Notify(Rinigin::EventArguments& arguments)
{
	if (GetContext()->GetCurrentState() != this) return;

	switch (arguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("SelectUp"):
		ShiftLetter(1);
		break;
	case Rinigin::Helpers::sdbm_hash("SelectDown"):
		ShiftLetter(-1);
		break;
	case Rinigin::Helpers::sdbm_hash("SelectLeft"):
		ShiftPosition(-1);
		break;
	case Rinigin::Helpers::sdbm_hash("SelectRight"):
		ShiftPosition(1);
		break;
	case Rinigin::Helpers::sdbm_hash("Confirm"): {
		if (not m_Saving and not m_Saved) {
			m_Saving = true;
			int score = m_GameState->GetLevel()->Score();
			HighScoreManager::SaveHighScores("highscore.json", { m_Abbreviation, score });
			m_Saved = true;
			m_Saving = false;
		}
		break;
	}
	default:
		break;
	}
}

void ScoreRegisterScreenState::UpdateInitialsField()
{
	m_InitialsEnterTextComponent->SetText(m_Abbreviation.c_str());
}

void ScoreRegisterScreenState::ShiftPosition(int index)
{
	m_SelectedLetter = std::min(std::max((m_SelectedLetter + index), 0), (DIGGER::MAX_LETTERS - 1));
	std::cout << m_SelectedLetter << std::endl;
}

void ScoreRegisterScreenState::ShiftLetter(int index)
{
	if (m_Abbreviation.empty())
	{
		m_Abbreviation = "___"; // start from blanks if nothing is set yet
	}

	if (m_SelectedLetter < 0 || m_SelectedLetter >= static_cast<int>(m_Abbreviation.size()))
		return;

	char current = m_Abbreviation[m_SelectedLetter];

	if (current < 'A' || current > 'Z') current = 'Z';

	int alphaIndex = (current - 'A' + index + 26) % 26;
	char newChar = static_cast<char>('A' + alphaIndex);

	m_Abbreviation[m_SelectedLetter] = newChar;

	UpdateInitialsField();
}
