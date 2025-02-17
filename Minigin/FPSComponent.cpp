#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent(GameObject* gameObject, TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
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
	m_TextComponent->SetText(FPS_Text + " FPS");
}
