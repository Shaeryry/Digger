#pragma once
#include "Component.h"
#include "Observer.h"
#include "Event.h"

namespace dae {
	class ScoreComponent final : public Component, public Observer
	{
	public: 
		explicit ScoreComponent(GameObject* gameObject);
		void virtual Notify(EventType eventType,GameObject* gameObject) override;

		int GetScore() const { return m_Score; };
		Event* GetScoreChangedEvent() const { return m_ScoreChangedEvent.get(); };
	private:
		void AddScore(int amount);

		std::unique_ptr<Event> m_ScoreChangedEvent;
		int m_Score;
	};
}

