#include <SDL.h>
#include <steam_api.h>
#include <iostream>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Achievement.h"
#include "Scene.h"

// Commands
#include "MovementCommands.h"
#include "KillEnemyCommand.h"
#include "DamageCommand.h"
// Components
#include "TransformComponent.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "CacheGraphComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreDisplayComponent.h"

dae::GameObject* CreatePlayer(dae::Scene& scene, bool keyboard, int playerIndex) {
	auto& input = dae::InputManager::GetInstance();

	dae::GameObject* gameObject{ nullptr };
	const float spawnOffset{ 50.f * static_cast<float>(playerIndex) };
	const float movementSpeed{ 300.f };
	int gamepadIndex{ playerIndex };

	// Setup gamepad
	auto* gamepad = input.GetGamepad(gamepadIndex);

	// Set up gameObject
	auto player = std::make_unique<dae::GameObject>();
	gameObject = player.get(); // Get pointer to the gameObject

	auto* playerRenderer = player->AddComponent<dae::TextureRendererComponent>();
	auto* scoreComponent = player->AddComponent<dae::ScoreComponent>();
	player->AddComponent<dae::TextureComponent>(playerRenderer, "VNOB1.BMP");
	player->AddComponent<dae::HealthComponent>(3);

	
	player->SetPosition({ (216 + spawnOffset) ,180,0 });

	// Commands

	auto* moveUpCommand{ input.AddCommand<dae::MovementCommand>(gameObject,glm::vec3{0,-1,0},movementSpeed) };
	auto* moveDownCommand{ input.AddCommand<dae::MovementCommand>(gameObject,glm::vec3{0,1,0},movementSpeed) };
	auto* moveLeftCommand{ input.AddCommand<dae::MovementCommand>(gameObject,glm::vec3{-1,0,0},movementSpeed) };
	auto* moveRightCommand{ input.AddCommand<dae::MovementCommand>(gameObject,glm::vec3{1,0,0},movementSpeed) };

	auto* damagePlayerCommand{ input.AddCommand<dae::DamageCommand>(gameObject) };
	auto* killEnemyCommand{ dynamic_cast<dae::KillEnemyCommand*>(input.AddCommand<dae::KillEnemyCommand>()) };

	// Setup bindings
	if (keyboard) {
		gamepad->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommand);
		gamepad->AddBinding(SDL_SCANCODE_A, dae::BindingConnection::OnHeld, moveLeftCommand);
		gamepad->AddBinding(SDL_SCANCODE_S, dae::BindingConnection::OnHeld, moveDownCommand);
		gamepad->AddBinding(SDL_SCANCODE_D, dae::BindingConnection::OnHeld, moveRightCommand);

		gamepad->AddBinding(SDL_SCANCODE_X, dae::BindingConnection::OnTrigger, killEnemyCommand);
		gamepad->AddBinding(SDL_SCANCODE_F, dae::BindingConnection::OnTrigger, damagePlayerCommand);
	}
	else {
		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, dae::BindingConnection::OnHeld, moveUpCommand);
		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, dae::BindingConnection::OnHeld, moveLeftCommand);
		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, dae::BindingConnection::OnHeld, moveDownCommand);
		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, dae::BindingConnection::OnHeld, moveRightCommand);

		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_A, dae::BindingConnection::OnTrigger, killEnemyCommand);
		gamepad->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_B, dae::BindingConnection::OnTrigger, damagePlayerCommand);
	}

	// Add Observers
	killEnemyCommand->GetEnemyDiedEvent()->AddObserver(scoreComponent);

	// Add to scene
	scene.Add(player); // Add player to the scene
	return gameObject;
}

dae::GameObject* InitalizePlayerAndUI(dae::Scene& scene,bool keyboard,int playerIndex) {
	auto& achievement = dae::Achievement::GetInstance();

	auto* player{ CreatePlayer(scene,keyboard,playerIndex) };
	auto* healthComponent{ player->GetComponent<dae::HealthComponent>() };
	auto* scoreComponent{ player->GetComponent<dae::ScoreComponent>() };

	auto* font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	const float offset{ 50.f * playerIndex };
	const glm::vec3 UIPosition{ 10,(150.f + offset),0 };

	// Create UI

	// Add lives
	auto livesGO = std::make_unique<dae::GameObject>();
	auto livesTextRenderer = livesGO->AddComponent<dae::TextureRendererComponent>();
	auto livesGOTextComponent = livesGO->AddComponent<dae::TextComponent>(livesTextRenderer, "", font);
	auto healthDisplayComponent = livesGO->AddComponent<dae::HealthDisplayComponent>(livesGOTextComponent);

	healthComponent->GetHealthChangedEvent()->AddObserver(healthDisplayComponent); // Allow this thing to listen


	// Add Score
	auto scoreGO = std::make_unique<dae::GameObject>();
	auto scoreTextRenderer = scoreGO->AddComponent<dae::TextureRendererComponent>();
	auto scoreTextComponent = scoreGO->AddComponent<dae::TextComponent>(scoreTextRenderer, "N/A", font);
	auto scoreDisplayComponent = scoreGO->AddComponent<dae::ScoreDisplayComponent>(scoreTextComponent);

	scoreComponent->GetScoreChangedEvent()->AddObserver(scoreDisplayComponent);
	scoreComponent->GetScoreChangedEvent()->AddObserver(&achievement);

	// Position UI Elements
	livesGO->SetPosition(UIPosition + glm::vec3{ 0, 20, 0 });
	scoreGO->SetPosition(UIPosition + glm::vec3{ 0, 40, 0 });


	scene.Add(livesGO);
	scene.Add(scoreGO);

	return player;
}

void load()
{
	//auto& input = dae::InputManager::GetInstance();
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fontLower = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);

	// WEEK 1
	auto background = std::make_unique<dae::GameObject>();
	auto backgroundRenderer = background->AddComponent<dae::TextureRendererComponent>();
	background->AddComponent<dae::TextureComponent>(backgroundRenderer,"background.tga");
	scene.Add(background);

	auto logo = std::make_unique<dae::GameObject>();
	auto logoRenderer = logo->AddComponent<dae::TextureRendererComponent>();
	logo->AddComponent<dae::TextureComponent>(logoRenderer,"logo.tga");
	logo->SetPosition({ 216, 180,0 });
	scene.Add(logo);
	 
	auto textObject = std::make_unique<dae::GameObject>();
	auto textRenderer = textObject->AddComponent<dae::TextureRendererComponent>();
	textObject->AddComponent<dae::TextComponent>(textRenderer,"Programming 4 Assignment",font);
	textObject->SetPosition({ 80, 20, 0 });
	scene.Add(textObject); 

	auto FPSTextObject = std::make_unique<dae::GameObject>();
	auto FPStextRenderer = FPSTextObject->AddComponent<dae::TextureRendererComponent>();
	auto textComponent = FPSTextObject->AddComponent<dae::TextComponent>(FPStextRenderer,font);
	FPSTextObject->AddComponent<dae::FPSComponent>(textComponent);
	scene.Add(FPSTextObject);

	// WEEK 2

	/*auto rootRootObject = std::make_unique<dae::GameObject>();
	rootRootObject->SetPosition({ 216,180,0 });

	auto rootObject = std::make_unique<dae::GameObject>(rootRootObject.get());
	auto rootObjectRenderer = rootObject->AddComponent<dae::TextureRendererComponent>();
	rootObject->AddComponent<dae::TextureComponent>(rootObjectRenderer, "VNOB1.BMP");
	rootObject->AddComponent<dae::RotatorComponent>(80.f);
	rootObject->SetPosition({ 216,180,0 });
	
	auto childObject = std::make_unique<dae::GameObject>(rootObject.get());
	auto childObjectRenderer = childObject->AddComponent<dae::TextureRendererComponent>();
	childObject->AddComponent<dae::TextureComponent>(childObjectRenderer, "VNOB1.BMP");
	childObject->AddComponent<dae::RotatorComponent>(40.f,.85f,-1.f);
	childObject->SetPosition({ 0,20,0 });

	scene.Add(childObject);
	scene.Add(rootObject);
	scene.Add(rootRootObject);*/

	// WEEK 3

	/*auto graphGO = std::make_unique<dae::GameObject>();
	graphGO->AddComponent<dae::CacheGraphComponent>();

	scene.Add(graphGO);*/

	// WEEK 4
	//auto keyboard = input.GetGamepad(0); // Keyboard
	//auto controller = input.GetGamepad(1);

	//auto playerController = std::make_unique<dae::GameObject>();
	//auto playerControllerRenderer = playerController->AddComponent<dae::TextureRendererComponent>();
	//playerController->AddComponent<dae::TextureComponent>(playerControllerRenderer, "VNOB1.BMP");
	//playerController->SetPosition({ 216,180,0 });
	//

	//auto playerKeyboard = std::make_unique<dae::GameObject>();
	//auto playerKeyboardRenderer = playerKeyboard->AddComponent<dae::TextureRendererComponent>();
	//auto healthComponent = playerKeyboard->AddComponent<dae::HealthComponent>(3);
	//auto scoreComponent = playerKeyboard->AddComponent<dae::ScoreComponent>();
	//playerKeyboard->AddComponent<dae::TextureComponent>(playerKeyboardRenderer, "VNOB1.BMP");
	//playerKeyboard->SetPosition({ 400,180,0 });

	//const float speed{ 300.f };
	//auto moveUpCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{0,-1,0},speed)) };
	//auto moveDownCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{0,1,0},speed)) };
	//auto moveLeftCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{-1,0,0},speed)) };
	//auto moveRightCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{1,0,0},speed)) };

	//auto moveUpCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{0,-1,0},speed*2)) };
	//auto moveDownCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{0,1,0},speed*2)) };
	//auto moveLeftCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{-1,0,0},speed*2)) };
	//auto moveRightCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{1,0,0},speed*2)) };

	//auto killEnemyCommand{ dynamic_cast<dae::KillEnemyCommand*>(input.AddCommand<dae::KillEnemyCommand>()) };
	//auto damagePlayerCommand{ dynamic_cast<dae::DamageCommand*>(input.AddCommand<dae::DamageCommand>(playerKeyboard.get())) };

	//// Bind commands controller

	//keyboard->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommandKeyboard);
	//keyboard->AddBinding(SDL_SCANCODE_S, dae::BindingConnection::OnHeld, moveDownCommandKeyboard);
	//keyboard->AddBinding(SDL_SCANCODE_A, dae::BindingConnection::OnHeld, moveLeftCommandKeyboard);
	//keyboard->AddBinding(SDL_SCANCODE_D, dae::BindingConnection::OnHeld, moveRightCommandKeyboard);
	//keyboard->AddBinding(SDL_SCANCODE_X, dae::BindingConnection::OnTrigger, killEnemyCommand);
	//keyboard->AddBinding(SDL_SCANCODE_F, dae::BindingConnection::OnTrigger, damagePlayerCommand);


	//controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP,dae::BindingConnection::OnHeld,moveUpCommandController);
	//controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, dae::BindingConnection::OnHeld, moveDownCommandController);
	//controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, dae::BindingConnection::OnHeld, moveLeftCommandController);
	//controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, dae::BindingConnection::OnHeld, moveRightCommandController);

	//scene.Add(playerController);
	//scene.Add(playerKeyboard);


	// WEEK 5

	//auto livesGO = std::make_unique<dae::GameObject>();
	//auto livesTextRenderer = livesGO->AddComponent<dae::TextureRendererComponent>();
	//auto livesGOTextComponent = livesGO->AddComponent<dae::TextComponent>(livesTextRenderer, "", font);
	//auto healthDisplayComponent = livesGO->AddComponent<dae::HealthDisplayComponent>(livesGOTextComponent);

	//healthComponent->GetHealthChangedEvent()->AddObserver(healthDisplayComponent); // Allow this thing to listen

	//livesGO->SetPosition({ 10, 60, 0 });
	//scene.Add(livesGO);

	//auto scoreGO = std::make_unique<dae::GameObject>();
	//auto scoreTextRenderer = scoreGO->AddComponent<dae::TextureRendererComponent>();
	//auto scoreTextComponent = scoreGO->AddComponent<dae::TextComponent>(scoreTextRenderer, "N/A", font);
	//auto scoreDisplayComponent = scoreGO->AddComponent<dae::ScoreDisplayComponent>(scoreTextComponent);
	//scoreGO->SetPosition({ 10, 100, 0 });

	//scoreComponent->GetScoreChangedEvent()->AddObserver(scoreDisplayComponent);
	//killEnemyCommand->GetEnemyDiedEvent()->AddObserver(scoreComponent);
	//scene.Add(scoreGO);

	auto tutorialKeyboard = std::make_unique<dae::GameObject>();
	auto tutorialKeyboardTextRenderer = tutorialKeyboard->AddComponent<dae::TextureRendererComponent>();
	auto tutorialKeyboardTextComponent = tutorialKeyboard->AddComponent<dae::TextComponent>(tutorialKeyboardTextRenderer, fontLower);
	tutorialKeyboardTextComponent->SetText("Use WASD to move Player 1, X to kill enemies and F to inflict damage on yourself.");
	tutorialKeyboard->SetPosition({ 0,80,0 });
	scene.Add(tutorialKeyboard);

	auto tutorialController = std::make_unique<dae::GameObject>();
	auto tutorialControllerTextRenderer = tutorialController->AddComponent<dae::TextureRendererComponent>();
	auto tutorialControllerTextComponent = tutorialController->AddComponent<dae::TextComponent>(tutorialControllerTextRenderer, fontLower);
	tutorialControllerTextComponent->SetText("Use the D-Pad to move Player 2, A to kill enemies and B to inflict damage on yourself.");
	tutorialController->SetPosition({ 0,100,0 });
	scene.Add(tutorialController);


	InitalizePlayerAndUI(scene, true, 0);
	InitalizePlayerAndUI(scene, false, 1);
}

int main(int, char* []) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else {
		std::cout << "Successfully initialized steam." << std::endl;
	}

	dae::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();
    return 0;
} 