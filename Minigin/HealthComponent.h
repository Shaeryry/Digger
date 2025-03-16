#pragma once
#include <memory>
#include "Component.h"
#include "Event.h"


namespace dae {
	class HealthComponent final : public Component
	{
	public:
		explicit HealthComponent(GameObject* gameObject, int maxHealth = 3,int minHealth = 0);
		void TakeDamage(int damage);

		void SetMaxHealth(int maxHealth);
		void SetMinHealth(int minHealth);

		int GetHealth() const { return m_Health; };
		int GetMinHealth() const { return m_MinHealth; };
		int GetMaxHealth() const { return m_MaxHealth; };

		Event* GetDiedEvent() const { return m_DiedEvent.get(); };
		Event* GetHealthChangedEvent() const { return m_HealthChangedEvent.get(); };
	private:
		void SetHealth(int health);

		std::unique_ptr<Event> m_DiedEvent;
		std::unique_ptr<Event> m_HealthChangedEvent;  

		int m_Health;
		int m_MinHealth;
		int m_MaxHealth;

	};
}

