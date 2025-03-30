#pragma once
#include <memory>
#include "Component.h"
#include "Event.h"


class HealthComponent final : public Rinigin::Component
{
public:
	explicit HealthComponent(Rinigin::GameObject* gameObject, int maxHealth = 3,int minHealth = 0);
	void TakeDamage(int damage);

	void SetMaxHealth(int maxHealth);
	void SetMinHealth(int minHealth);

	int GetHealth() const { return m_Health; };
	int GetMinHealth() const { return m_MinHealth; };
	int GetMaxHealth() const { return m_MaxHealth; };

	Rinigin::Event* GetDiedEvent() const { return m_DiedEvent.get(); };
	Rinigin::Event* GetHealthChangedEvent() const { return m_HealthChangedEvent.get(); };
private:
	void SetHealth(int health);

	std::unique_ptr<Rinigin::Event> m_DiedEvent;
	std::unique_ptr<Rinigin::Event> m_HealthChangedEvent;  

	int m_Health;
	int m_MinHealth;
	int m_MaxHealth;
};


