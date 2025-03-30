#pragma once
#include "Command.h"


class HealthComponent;
namespace Rinigin {
	class GameObject;
};

class DamageCommand final : public Rinigin::Command
{
public:
	explicit DamageCommand(Rinigin::GameObject* gameObject);
	virtual void Execute() override;
private:
	Rinigin::GameObject* m_GameObject;
	HealthComponent* m_HealthComponent;
};

