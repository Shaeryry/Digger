#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
	class ScoreDisplayComponent : public Component, public Observer
	{
	public:
		explicit ScoreDisplayComponent(GameObject* gameObject,TextComponent* textComponent);
		virtual void Notify(EventType eventType,GameObject* gameObject) override;
	private:
		void UpdateScore(int score);
		TextComponent* m_TextComponent;
	};
}


