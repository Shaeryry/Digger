#pragma once
#include "Command.h"

namespace dae {
	class HealthComponent;
	class GameObject;
	class DamageCommand final : public Command
	{
	public:
		explicit DamageCommand(GameObject* gameObject);
		virtual void Execute() override;
	private:
		GameObject* m_GameObject;
		HealthComponent* m_HealthComponent;
	};
}

