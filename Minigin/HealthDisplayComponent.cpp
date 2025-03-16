#include "HealthDisplayComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* gameObject, TextComponent* textComponent) :
	Component(gameObject),
	m_TextComponent{ textComponent }
{
	UpdateText(3);
}

void dae::HealthDisplayComponent::Notify(EventType event, GameObject* gameObject)
{
	switch (event)
	{
	case dae::EventType::HealthChanged:
		auto* healthComponent{ gameObject->GetComponent<dae::HealthComponent>() };
		if (healthComponent) {
			UpdateText( healthComponent->GetHealth() );
		}
		break;
	}
}

void dae::HealthDisplayComponent::UpdateText(int currentHealth)
{
	m_TextComponent->SetText("Health Remaining : " + std::to_string(currentHealth));
}
