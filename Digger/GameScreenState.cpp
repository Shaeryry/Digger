#include "GameScreenState.h"

#include <iostream>
#include <fstream>

#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "TextureRendererComponent.h"
#include "GameObject.h"
#include "Character.h"
#include "DiggerMobile.h"
#include "Gamepad.h"
#include "MovementCommands.h" 
#include "Scene.h"
#include "DamageCommand.h"
#include "TextComponent.h"
#include "TerrainComponent.h"
#include "DiggerGame.h"
#include "RigidbodyComponent.h"

#include "ScoreComponent.h"
#include "Emerald.h"
#include "MoneyBag.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

GameScreenState::GameScreenState(Rinigin::StateContextComponent* context, DiggerGame* game) :
	Rinigin::State(context),
	m_Game(game),
	m_Scene(Rinigin::SceneManager::GetInstance().GetActiveScene()),
	m_GameMode(GameMode::SOLO),
	m_MapComponent(nullptr)
{
	glm::vec2 screenSize = m_Game->GetScreenSize();

	m_BackgroundGameObject = m_Scene->CreateObject();
	m_MapComponent = m_BackgroundGameObject->AddComponent<TerrainComponent>(screenSize,screenSize,40);

	Rinigin::Gamepad* playerOneGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(0) }; // Keyboard
	//Rinigin::Gamepad* playerTwoGamepad{ Rinigin::InputManager::GetInstance().GetGamepad(1) };

	// TODO : Create characters
	m_DiggerOne = dynamic_cast<DiggerMobile*>(AddCharacter( new DiggerMobile(0, m_MapComponent) ));
	//m_DiggerOne->GetCharacterObject()->SetPosition(screenSize.x / 2, screenSize.y / 2, 0);
	m_DiggerOne->GetRigidbody()->GravityEnabled(false);

	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Down, m_DiggerOne->UpDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Down, m_DiggerOne->DownDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Down, m_DiggerOne->LeftDirectionCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Down, m_DiggerOne->RightDirectionCommand());

	playerOneGamepad->AddBinding(SDL_SCANCODE_UP, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_DOWN, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_LEFT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());
	playerOneGamepad->AddBinding(SDL_SCANCODE_RIGHT, Rinigin::BindingConnection::Held, m_DiggerOne->GetMoveCommand());

	// TEMPORARY WAY OF DAMAGING YOUR OWN CHARACTER
	playerOneGamepad->AddBinding(SDL_SCANCODE_F, Rinigin::BindingConnection::Down, m_DiggerOne->GetDamageCommand());

	m_DiggerTwo = dynamic_cast<DiggerMobile*>(AddCharacter(new DiggerMobile(1,m_MapComponent)));
	//m_DiggerTwo->GetCharacterObject()->SetPosition(screenSize.x / 2, screenSize.y / 2, 0);

	// TEMPORARY DISPLAY
} 


void GameScreenState::Enter()
{
	Reset();
	LoadLevel(1);
	// TODO : Generate level

	switch (m_GameMode)
	{
		case GameMode::SOLO:
			StartSolo();
			break;
		case GameMode::COOP:
			StartCoop();
			break;
		case GameMode::PVP:
			// TODO : Spawn 1 player digger and 1 player enemy
			break;
	default:
		break;
	}
}

Rinigin::State* GameScreenState::Update()
{
	return nullptr;
}

void GameScreenState::Exit()
{
	Reset();
	// TODO : Remove bindings
	std::cout << "Game ended !" << std::endl;
}


glm::vec2& GameScreenState::GetPlayerSpawnIndex(int playerIndex)
{
	return m_LevelData.playerSpawns[playerIndex % m_LevelData.playerSpawns.size()];
}

void GameScreenState::Reset()
{
	m_DiggerOne->GetCharacterObject()->SetActive(false); 
	m_DiggerTwo->GetCharacterObject()->SetActive(false);
}

void GameScreenState::LoadLevel(int levelIndex)
{
	std::cout << "level loader" << std::endl;

	const std::string levelName{ "LVL" + std::to_string(levelIndex) };
	std::string levelResourcesPath{ "../Data/Levels/" };
	std::string levelTexturePath{ levelResourcesPath + "Textures/" + levelName + ".png" };
	std::string levelDataPath{ levelResourcesPath + "Data/" + levelName + ".json" };

	/*auto* font = Rinigin::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	 
	auto* textGO = m_Scene->CreateObject();
	auto* textRenderer = textGO->AddComponent<Rinigin::TextureRendererComponent>();
	auto* text = textGO->AddComponent<Rinigin::TextComponent>(textRenderer, "Press 'F' to kill the digger mobile.", font);
	text->SetColor(255, 255, 255);
	textGO->SetActive(true);
	textGO->SetPosition(0, 0, 0);*/

	// Load Texture
	LoadLevelFile(levelDataPath.c_str());
	m_MapComponent->ChangeBackgroundTexture(levelTexturePath.c_str());

	InitializeLevel();
}

void GameScreenState::LoadLevelFile(const char* filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) {
		std::cerr << "Failed to open " << filePath << "\n";
		return;
	}

	json j;
	ifs >> j;

	m_LevelData.width = j["width"];
	m_LevelData.height = j["height"];

	const auto& tilesJson = j["tiles"];
	int totalTileCount{ m_LevelData.width * m_LevelData.height };

	if (tilesJson.size() != totalTileCount) {
		std::cerr << "Tile count mismatch.\n";
		return;
	}

	m_LevelData.tiles.resize(totalTileCount);

	for (size_t tileIndex{ 0 }; tileIndex < tilesJson.size(); tileIndex++) {
		int tileValue = tilesJson[tileIndex];
		m_LevelData.tiles[tileIndex] = tileValue;
	}
}

void GameScreenState::InitializeLevel()
{
	glm::vec2 screenSize = m_Game->GetScreenSize();
	
	const int tileSize{ 32 };
	int tileWidth{ static_cast<int>(screenSize.x / m_LevelData.width) };
	int tileHeight{ static_cast<int>(screenSize.y / m_LevelData.height) };

	for (int x{ 0 }; x < m_LevelData.width; x++) {
		for (int y{ 0 }; y < m_LevelData.height; y++) {

			const int tileIndex{ y * m_LevelData.width + x };
			const unsigned int tileValue{ m_LevelData.tiles[tileIndex] };
			const glm::vec2 tilePos{x * tileWidth, y * tileHeight};

			switch (tileValue)
			{
			case 0:
				// Default nothing
				break;
			case 1:
				// Dug out
				m_MapComponent->DigAt(tilePos.x, tilePos.y, tileSize);
				break;
			case 2:
				// Spawn points
				m_LevelData.playerSpawns.emplace_back(tilePos);
				m_MapComponent->DigAt(tilePos.x, tilePos.y, tileSize);
				break;
			case 3: {
				// Emeralds
				Emerald* emerald = CreateItem<Emerald>(glm::vec2(tilePos.x, tilePos.y));
				emerald->GetCollectedEvent()->AddObserver(m_DiggerOne->GetScoreComponent());
				emerald->GetCollectedEvent()->AddObserver(m_DiggerTwo->GetScoreComponent());
				break;
			}
			case 4:
				// Enemy spawn
			case 5:
				// Money bag
				CreateItem<MoneyBag>(glm::vec2(tilePos.x, tilePos.y));
				break;
			default:
				break;
			}

		}
	}
}

void GameScreenState::StartSolo()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	m_DiggerOne->GetCharacterObject()->SetPosition( GetPlayerSpawnIndex(0) );
	std::cout << "Solo game!" << std::endl;
}

void GameScreenState::StartCoop()
{
	m_DiggerOne->GetCharacterObject()->SetActive(true);
	m_DiggerOne->GetCharacterObject()->SetPosition(GetPlayerSpawnIndex(0));

	m_DiggerTwo->GetCharacterObject()->SetActive(true);
	m_DiggerTwo->GetCharacterObject()->SetPosition(GetPlayerSpawnIndex(1));

}

Character* GameScreenState::AddCharacter(Character* character)
{ 
	std::unique_ptr<Character> characterPointer(character);
	m_Characters.emplace_back( std::move(characterPointer) );
	return m_Characters.back().get();
}