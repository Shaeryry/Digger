#include <iomanip>
#include "GameObject.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent(GameObject* gameObject, Font* font) :
	TextComponent(gameObject, font)
{
}

void dae::FPSComponent::Update()
{
	float FPS{ Timer::GetInstance().GetFPS() };

	// Set precision of the FPS
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << FPS;
	std::string formattedFPS = ss.str();


	const std::string FPS_Text{ formattedFPS };
	SetText(FPS_Text + " FPS");

	TextComponent::Update();
}
