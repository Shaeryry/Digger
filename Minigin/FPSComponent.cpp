#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Timer.h"

dae::FPSComponent::FPSComponent() :
	m_assignedTextComponent{ nullptr }
{
}

dae::FPSComponent::FPSComponent(TextComponent* textComponent)
{
	AssignTextComponent(textComponent);
}

void dae::FPSComponent::AssignTextComponent(TextComponent* textComponent)
{
	m_assignedTextComponent = textComponent;
}

void dae::FPSComponent::Update() 
{
	float FPS{ Timer::GetInstance().GetFPS() };

	// Set precision of the FPS
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << FPS;
	std::string formattedFPS = ss.str();


	const std::string FPS_Text{ formattedFPS };
	m_assignedTextComponent->SetText( FPS_Text + " FPS" );
}
