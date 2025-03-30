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

void HealthDisplayComponent::Notify(Rinigin::EventArguments* eventArgs)
{
	switch (eventArgs->GetID())
	{
		case Rinigin::Helpers::sdbm_hash("HealthChanged") : {
			auto arguments = GetArgumentsOfType<GameObjectEventArguments>(eventArgs);
			auto* healthComponent{ arguments->GetGameObject()->GetComponent<HealthComponent>() };
			if (healthComponent) {
				UpdateText(healthComponent->GetHealth());
			}
			break;
		}
			
	}

}

void HealthDisplayComponent::UpdateText(int currentHealth)
{
	m_TextComponent->SetText("Health Remaining : " + std::to_string(currentHealth));
}
