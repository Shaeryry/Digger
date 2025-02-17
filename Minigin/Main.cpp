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
#include "Scene.h"

// Components
#include "TransformComponent.h"
#include "TextureRendererComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// WEEK 1
	auto background = std::make_unique<dae::GameObject>();
	auto backgroundRenderer = background->AddComponent<dae::TextureRendererComponent>();
	background->AddComponent<dae::TextureComponent>(backgroundRenderer,"background.tga");
	scene.Add(background);

	/*auto logo = std::make_unique<dae::GameObject>();
	auto logoRenderer = logo->AddComponent<dae::TextureRendererComponent>();
	logo->AddComponent<dae::TextureComponent>(logoRenderer,"logo.tga");
	logo->GetComponent<dae::TransformComponent>()->SetWorldPosition(216, 180,0);
	scene.Add(logo);*/
	 
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textObject = std::make_unique<dae::GameObject>();
	auto textRenderer = textObject->AddComponent<dae::TextureRendererComponent>();
	textObject->AddComponent<dae::TextComponent>(textRenderer,"Programming 4 Assignment",font);
	textObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(80, 20, 0);
	scene.Add(textObject); 

	auto FPSTextObject = std::make_unique<dae::GameObject>();
	auto FPStextRenderer = FPSTextObject->AddComponent<dae::TextureRendererComponent>();
	auto textComponent = FPSTextObject->AddComponent<dae::TextComponent>(FPStextRenderer,font);
	FPSTextObject->AddComponent<dae::FPSComponent>(textComponent);
	scene.Add(FPSTextObject);

	// WEEK 2

	auto rootRootObject = std::make_unique<dae::GameObject>();
	rootRootObject->SetPosition({ 216,180,0 });

	auto rootObject = std::make_unique<dae::GameObject>(rootRootObject.get());
	auto rootObjectRenderer = rootObject->AddComponent<dae::TextureRendererComponent>();
	rootObject->AddComponent<dae::RotatorComponent>();
	rootObject->AddComponent<dae::TextureComponent>(rootObjectRenderer, "logo.tga");
	rootObject->SetPosition({ 216,180,0 });
	
	auto childObject = std::make_unique<dae::GameObject>(rootObject.get());
	auto childObjectRenderer = childObject->AddComponent<dae::TextureRendererComponent>();
	childObject->AddComponent<dae::RotatorComponent>(-1.f);
	childObject->AddComponent<dae::TextureComponent>(childObjectRenderer, "logo.tga");
	childObject->SetPosition({ 0,300,0 });

	scene.Add(childObject);
	scene.Add(rootObject);
	scene.Add(rootRootObject);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
} 