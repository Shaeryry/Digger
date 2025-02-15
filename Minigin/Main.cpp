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
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Scene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_unique<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(background);

	auto logo = std::make_unique<dae::GameObject>();
	logo->AddComponent<dae::TextureComponent>("logo.tga");
	logo->GetComponent<dae::TransformComponent>()->SetPosition(216, 180,0);
	scene.Add(logo);
	 
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textObject = std::make_unique<dae::GameObject>();
	textObject->AddComponent<dae::TextComponent>("Programming 4 Assignment",font);
	textObject->GetComponent<dae::TransformComponent>()->SetPosition(80, 20, 0);
	scene.Add(textObject);

	auto FPSTextObject = std::make_unique<dae::GameObject>();
	FPSTextObject->AddComponent<dae::FPSComponent>(font);
	FPSTextObject->GetComponent<dae::FPSComponent>()->SetColor(255, 255, 255);
	FPSTextObject->GetComponent<dae::FPSComponent>()->Destroy();
	scene.Add(FPSTextObject);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}