#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		explicit HealthDisplayComponent(GameObject* gameObject, TextComponent* textComponent);
		virtual void Notify(EventType event,GameObject* gameObject) override;
	private:
		void UpdateText(int currentHealth);
		TextComponent* m_TextComponent;
	};
}

