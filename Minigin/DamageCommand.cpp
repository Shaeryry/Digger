#include "DamageCommand.h"
#include "HealthComponent.h"
#include "GameObject.h"

DamageCommand::DamageCommand(Rinigin::GameObject* gameObject) :
	m_GameObject{ gameObject },
	m_HealthComponent{ nullptr }
{
	m_HealthComponent = m_GameObject->GetComponent<HealthComponent>();
}

void DamageCommand::Execute()
{
	if (m_HealthComponent) {
		m_HealthComponent->TakeDamage(1);
	}
}
