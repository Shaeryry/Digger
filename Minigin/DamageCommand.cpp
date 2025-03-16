#include "DamageCommand.h"
#include "HealthComponent.h"
#include "GameObject.h"

dae::DamageCommand::DamageCommand(GameObject* gameObject) :
	m_GameObject{ gameObject },
	m_HealthComponent{ nullptr }
{
	m_HealthComponent = m_GameObject->GetComponent<HealthComponent>();
}

void dae::DamageCommand::Execute()
{
	if (m_HealthComponent) {
		m_HealthComponent->TakeDamage(1);
	}
}
