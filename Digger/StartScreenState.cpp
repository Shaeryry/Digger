#include "StartScreenState.h"
#include <iostream>
#include "SceneManager.h"
#include "MenuCommand.h"
#include "Gamepad.h"
#include "Helpers.h"
#include "Scene.h"

#include "TextureComponent.h"
#include "TextureRendererComponent.h"
#include "LetterTextComponent.h"

#include "GameScreenState.h"

#include "DiggerConstants.h"
#include "StateContextComponent.h"
#include "HighScoreManager.h"

StartScreenState::StartScreenState(Rinigin::StateContextComponent* context, Rinigin::Gamepad* gamepad) :
	Rinigin::State(context),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),

	m_ScreenGameObject(nullptr), 
	m_BackgroundTextureComponent(nullptr),
	m_ScreenRenderer(nullptr),

	m_TitleGameObject(nullptr),
	m_TitleWordComponent(nullptr),
	m_MenuController(gamepad)
{
	// Setup game object
	m_GameScreenState = dynamic_cast<GameScreenState*>(GetContext()->GetState<GameScreenState>());

	// Background
	m_ScreenGameObject = m_Scene->CreateObject();
	m_ScreenGameObject->SetPosition(0, 0, 0);
	m_ScreenGameObject->SetActive(false);

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

	// HIGHSCORE
	m_HighscoreGameObject = m_Scene->CreateObject(m_ScreenGameObject);
	m_HighscoreTextComponent = m_HighscoreGameObject->AddComponent<LetterTextComponent>("HIGH SCORE");
	m_HighscoreTextComponent->SetColor({ 255,0,0 });
	m_HighscoreGameObject->SetPosition(15, 50, 0);

	// Game Objects
	for (int entryIndex{ 0 }; entryIndex < DIGGER::MAX_HIGHSCORE_ENTRIES; entryIndex++) {
		Rinigin::GameObject* gameObjectInitials = m_Scene->CreateObject(m_ScreenGameObject);
		Rinigin::GameObject* gameObjectScore = m_Scene->CreateObject(m_ScreenGameObject);

		LetterTextComponent* initialTextComponent = gameObjectInitials->AddComponent<LetterTextComponent>("...");
		LetterTextComponent* scoreTextComponent = gameObjectScore->AddComponent<LetterTextComponent>("0");

		m_HighscoreInitialTextComponents.emplace_back(initialTextComponent);
		m_HighscoreScoreTextComponents.emplace_back(scoreTextComponent);
		m_HighscoreInitialsGameObjects.emplace_back(gameObjectInitials);
		m_HighscoreScoreGameObjects.emplace_back(gameObjectScore);

	}
}  

void StartScreenState::Enter() 
{
	std::vector<HighScoreEntry> scores = HighScoreManager::LoadHighScores("highscore.json");
	for (int entryIndex{ 0 }; entryIndex < DIGGER::MAX_HIGHSCORE_ENTRIES; entryIndex++) {
		const HighScoreEntry& entry = scores[entryIndex];
		
		Rinigin::GameObject* gameObjectInitials = m_HighscoreInitialsGameObjects[entryIndex];
		Rinigin::GameObject* gameObjectScore = m_HighscoreScoreGameObjects[entryIndex];

		LetterTextComponent* initialTextComponent = m_HighscoreInitialTextComponents[entryIndex];
		LetterTextComponent* scoreTextComponent = m_HighscoreScoreTextComponents[entryIndex];

		if (entryIndex == 0) {
			initialTextComponent->SetColor({ 255,0,0 });
			scoreTextComponent->SetColor({ 255,0,0 });
		}

		const float y{ 50.f + (DIGGER::SCORE_DISTANCE + 5.f) + (DIGGER::SCORE_DISTANCE * entryIndex) };

		gameObjectInitials->SetPosition((DIGGER::SCREEN_WIDTH / 2.f) - (scoreTextComponent->GetLength() / 2) - 75.f, y, 0);
		gameObjectScore->SetPosition(15, y, 0);

		initialTextComponent->SetText(entry.initials.c_str());
		scoreTextComponent->SetText(Rinigin::Helpers::GetFormattedScore(entry.score).c_str());
	}

	m_ScreenGameObject->SetActive(true);
	//std::cout << "entered start screen" << std::endl;
	SetSelection(0);
}

Rinigin::State* StartScreenState::Update()
{
	return nullptr;
}

void StartScreenState::Exit()
{
	m_ScreenGameObject->SetActive(false);
	std::cout << "left start screen" << std::endl;
}

void StartScreenState::Notify(Rinigin::EventArguments & eventArguments)
{
	if (GetContext()->GetCurrentState() != this) return;
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
