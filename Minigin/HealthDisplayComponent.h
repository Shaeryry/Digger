#pragma once
#include "Component.h"
#include "Observer.h"

namespace Rinigin { class TextComponent; };
class HealthDisplayComponent final : public Rinigin::Component, public Rinigin::Observer
{
public:
	explicit HealthDisplayComponent(Rinigin::GameObject* gameObject, Rinigin::TextComponent* textComponent);
	virtual void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	void UpdateText(int currentHealth);
	Rinigin::TextComponent* m_TextComponent;
};

