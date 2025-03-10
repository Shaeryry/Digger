#include <SDL.h>

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
#include "Scene.h"

// Commands
#include "MovementCommands.h"
// Components
#include "TransformComponent.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "CacheGraphComponent.h"

void load()
{
	auto& input = dae::InputManager::GetInstance();
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

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
	 
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

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

	// WEEK 
	auto keyboard = input.GetGamepad(0); // Keyboard
	auto controller = input.GetGamepad(1);

	auto playerController = std::make_unique<dae::GameObject>();
	auto playerControllerRenderer = playerController->AddComponent<dae::TextureRendererComponent>();
	playerController->AddComponent<dae::TextureComponent>(playerControllerRenderer, "VNOB1.BMP");
	playerController->SetPosition({ 216,180,0 });

	auto playerKeyboard = std::make_unique<dae::GameObject>();
	auto playerKeyboardRenderer = playerKeyboard->AddComponent<dae::TextureRendererComponent>();
	playerKeyboard->AddComponent<dae::TextureComponent>(playerKeyboardRenderer, "VNOB1.BMP");
	playerKeyboard->SetPosition({ 400,180,0 });

	const float speed{ 300.f };
	auto moveUpCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{0,-1,0},speed)) };
	auto moveDownCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{0,1,0},speed)) };
	auto moveLeftCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{-1,0,0},speed)) };
	auto moveRightCommandController{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerController.get(),glm::vec3{1,0,0},speed)) };

	auto moveUpCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{0,-1,0},speed*2)) };
	auto moveDownCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{0,1,0},speed*2)) };
	auto moveLeftCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{-1,0,0},speed*2)) };
	auto moveRightCommandKeyboard{ dynamic_cast<dae::MovementCommand*>(input.AddCommand<dae::MovementCommand>(playerKeyboard.get(),glm::vec3{1,0,0},speed*2)) };

	// Bind commands controller

	keyboard->AddBinding(SDL_SCANCODE_W, dae::BindingConnection::OnHeld, moveUpCommandKeyboard);
	keyboard->AddBinding(SDL_SCANCODE_S, dae::BindingConnection::OnHeld, moveDownCommandKeyboard);
	keyboard->AddBinding(SDL_SCANCODE_A, dae::BindingConnection::OnHeld, moveLeftCommandKeyboard);
	keyboard->AddBinding(SDL_SCANCODE_D, dae::BindingConnection::OnHeld, moveRightCommandKeyboard);

	controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_UP,dae::BindingConnection::OnHeld,moveUpCommandController);
	controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_DOWN, dae::BindingConnection::OnHeld, moveDownCommandController);
	controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_LEFT, dae::BindingConnection::OnHeld, moveLeftCommandController);
	controller->AddBinding(dae::GamepadButton::MINIGIN_GAMEPAD_DPAD_RIGHT, dae::BindingConnection::OnHeld, moveRightCommandController);

	scene.Add(playerController);
	scene.Add(playerKeyboard);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
} 