#include <algorithm>
#include "HealthComponent.h"
#include "EventTypes.h"
	
HealthComponent::HealthComponent(Rinigin::GameObject* gameObject, int maxHealth, int minHealth) :
	Component(gameObject),
	m_MinHealth{ minHealth },
	m_MaxHealth{ maxHealth },
	m_Health{ maxHealth },
	m_DiedEvent{ std::make_unique<Rinigin::Event>( GameObjectEventArguments("Died",gameObject) )},
	m_HealthChangedEvent{ std::make_unique<Rinigin::Event>( GameObjectEventArguments("HealthChanged",gameObject)) }
{
}

void HealthComponent::TakeDamage(int damage)
{
	SetHealth(m_Health - damage);
	if (m_Health <= 0) { 
		m_DiedEvent->NotifyObservers();
	}
}

void HealthComponent::SetHealth(int health)
{
	const int currentHealth{ m_Health };
	const int newHealth{ std::clamp(health,m_MinHealth,m_MaxHealth) };

	m_Health = newHealth;
	if (currentHealth != newHealth) {
		m_HealthChangedEvent->NotifyObservers();
	}
}

void HealthComponent::SetMaxHealth(int maxHealth)
{
	m_MaxHealth = maxHealth;
	SetHealth(m_Health); // Update the health to clamp
}

void HealthComponent::SetMinHealth(int minHealth)
{
	m_MinHealth = minHealth;
	SetHealth(m_Health); // Update the health to clamp
}
