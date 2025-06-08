#include "HealthDisplayComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "EventTypes.h"
#include "Helpers.h"

HealthDisplayComponent::HealthDisplayComponent(Rinigin::GameObject* gameObject, Rinigin::TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
{
	UpdateText(3);
}

void HealthDisplayComponent::Notify(Rinigin::EventArguments & eventArguments)
{
	switch (eventArguments.GetID())
	{
		case 1:
			break;
		default:
			break;
	}

}

void HealthDisplayComponent::UpdateText(int currentHealth)
{
	m_TextComponent->SetText("Health Remaining : " + std::to_string(currentHealth));
}
