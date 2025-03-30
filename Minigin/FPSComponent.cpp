#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Timer.h"

FPSComponent::FPSComponent(Rinigin::GameObject* gameObject, Rinigin::TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
{
}

void FPSComponent::Update()
{
	float FPS{ Rinigin::Timer::GetInstance().GetFPS() };

	// Set precision of the FPS
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << FPS;
	std::string formattedFPS = ss.str();

	const std::string FPS_Text{ formattedFPS };
	m_TextComponent->SetText(FPS_Text + " FPS");
}
