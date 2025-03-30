#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "Minigin.h"
#include "Scene.h"

#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include "DamageCommand.h"
#include "MovementCommands.h"
#include "KillEnemyCommand.h"

#include "TextureRendererComponent.h"
#include"TextureComponent.h"
#include "TextComponent.h"

#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreDisplayComponent.h"
#include "FPSComponent.h"

Rinigin::GameObject* CreatePlayer(Rinigin::Scene& scene, bool keyboard, int playerIndex) {
	auto& input = Rinigin::InputManager::GetInstance();

	Rinigin::GameObject* gameObject{ nullptr };
	const float spawnOffset{ 50.f * static_cast<float>(playerIndex) };
	const float movementSpeed{ 300.f };
	int gamepadIndex{ 0 };

	// Setup gamepad
	if (!keyboard) gamepadIndex = 1;
	auto* gamepad = input.GetGamepad(gamepadIndex);

	// Set up gameObject
	auto player = std::make_unique<Rinigin::GameObject>();
	gameObject = player.get(); // Get pointer to the gameObject

	auto* playerRenderer = player->AddComponent<Rinigin::TextureRendererComponent>();
	auto* scoreComponent = player->AddComponent<ScoreComponent>();
	player->AddComponent<Rinigin::TextureComponent>(playerRenderer, "VNOB1.BMP");
	player->AddComponent<HealthComponent>(3);


	player->SetPosition({ (216 + spawnOffset) ,180,0 });

	// Commands

	auto* moveUpCommand{ input.AddCommand<MovementCommand>(gameObject,glm::vec3{0,-1,0},movementSpeed) };
	auto* moveDownCommand{ input.AddCommand<MovementCommand>(gameObject,glm::vec3{0,1,0},movementSpeed) };
	auto* moveLeftCommand{ input.AddCommand<MovementCommand>(gameObject,glm::vec3{-1,0,0},movementSpeed) };
	auto* moveRightCommand{ input.AddCommand<MovementCommand>(gameObject,glm::vec3{1,0,0},movementSpeed) };

	auto* damagePlayerCommand{ input.AddCommand<DamageCommand>(gameObject) };
	auto* killEnemyCommand{ dynamic_cast<KillEnemyCommand*>(input.AddCommand<KillEnemyCommand>()) };

	// Setup bindings
	if (keyboard) {
		gamepad->AddBinding(SDL_SCANCODE_W, Rinigin::BindingConnection::OnHeld, moveUpCommand);
		gamepad->AddBinding(SDL_SCANCODE_A, Rinigin::BindingConnection::OnHeld, moveLeftCommand);
		gamepad->AddBinding(SDL_SCANCODE_S, Rinigin::BindingConnection::OnHeld, moveDownCommand);
		gamepad->AddBinding(SDL_SCANCODE_D, Rinigin::BindingConnection::OnHeld, moveRightCommand);

		gamepad->AddBinding(SDL_SCANCODE_X, Rinigin::BindingConnection::OnTrigger, killEnemyCommand);
		gamepad->AddBinding(SDL_SCANCODE_F, Rinigin::BindingConnection::OnTrigger, damagePlayerCommand);
	}
	else {
		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP, Rinigin::BindingConnection::OnHeld, moveUpCommand);
		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, Rinigin::BindingConnection::OnHeld, moveLeftCommand);
		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, Rinigin::BindingConnection::OnHeld, moveDownCommand);
		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, Rinigin::BindingConnection::OnHeld, moveRightCommand);

		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_A, Rinigin::BindingConnection::OnTrigger, killEnemyCommand);
		gamepad->AddBinding(Rinigin::GamepadButton::MINIGIN_GAMEPAD_B, Rinigin::BindingConnection::OnTrigger, damagePlayerCommand);
	}

	// Add Observers
	killEnemyCommand->GetEnemyDiedEvent()->AddObserver(scoreComponent);

	// Add to scene
	scene.Add(player); // Add player to the scene 

	return gameObject;
}

Rinigin::GameObject* InitalizePlayerAndUI(Rinigin::Scene& scene, bool keyboard, int playerIndex) {
	auto* player{ CreatePlayer(scene,keyboard,playerIndex) };
	auto* healthComponent{ player->GetComponent<HealthComponent>() };
	auto* scoreComponent{ player->GetComponent<ScoreComponent>() };

	auto* font = Rinigin::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	const float offset{ 50.f * playerIndex };
	const glm::vec3 UIPosition{ 10,(60 + offset),0 };

	// Create UI

	// Add lives
	auto livesGO = std::make_unique<Rinigin::GameObject>();
	auto livesTextRenderer = livesGO->AddComponent<Rinigin::TextureRendererComponent>();
	auto livesGOTextComponent = livesGO->AddComponent<Rinigin::TextComponent>(livesTextRenderer, "", font);
	auto healthDisplayComponent = livesGO->AddComponent<HealthDisplayComponent>(livesGOTextComponent);

	healthComponent->GetHealthChangedEvent()->AddObserver(healthDisplayComponent); // Allow this thing to listen


	// Add Score
	auto scoreGO = std::make_unique<Rinigin::GameObject>();
	auto scoreTextRenderer = scoreGO->AddComponent<Rinigin::TextureRendererComponent>();
	auto scoreTextComponent = scoreGO->AddComponent<Rinigin::TextComponent>(scoreTextRenderer, "N/A", font);
	auto scoreDisplayComponent = scoreGO->AddComponent<ScoreDisplayComponent>(scoreTextComponent);

	scoreComponent->GetScoreChangedEvent()->AddObserver(scoreDisplayComponent);

	// Position UI Elements
	livesGO->SetPosition(UIPosition + glm::vec3{ 0, 20, 0 });
	scoreGO->SetPosition(UIPosition + glm::vec3{ 0, 40, 0 });


	scene.Add(livesGO);
	scene.Add(scoreGO);

	return player;
}


void load() {
	auto& scene = Rinigin::SceneManager::GetInstance().CreateScene("Demo");

	// WEEK 1
	auto background = std::make_unique<Rinigin::GameObject>();
	auto backgroundRenderer = background->AddComponent<Rinigin::TextureRendererComponent>();
	background->AddComponent<Rinigin::TextureComponent>(backgroundRenderer, "background.tga");
	scene.Add(background);

	auto logo = std::make_unique<Rinigin::GameObject>();
	auto logoRenderer = logo->AddComponent<Rinigin::TextureRendererComponent>();
	logo->AddComponent<Rinigin::TextureComponent>(logoRenderer, "logo.tga");
	logo->SetPosition({ 216, 180,0 });
	scene.Add(logo);

	auto font = Rinigin::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textObject = std::make_unique<Rinigin::GameObject>();
	auto textRenderer = textObject->AddComponent<Rinigin::TextureRendererComponent>();
	textObject->AddComponent<Rinigin::TextComponent>(textRenderer, "Programming 4 Assignment", font);
	textObject->SetPosition({ 80, 20, 0 });
	scene.Add(textObject);

	auto FPSTextObject = std::make_unique<Rinigin::GameObject>();
	auto FPStextRenderer = FPSTextObject->AddComponent<Rinigin::TextureRendererComponent>();
	auto textComponent = FPSTextObject->AddComponent<Rinigin::TextComponent>(FPStextRenderer, font);
	FPSTextObject->AddComponent<FPSComponent>(textComponent);
	scene.Add(FPSTextObject);


	InitalizePlayerAndUI(scene,true,0);
	InitalizePlayerAndUI(scene, false, 1);
}

int main(int, char* []) {
	Rinigin::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
} 