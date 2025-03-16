#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* gameObject) :
	Component(gameObject),
	m_Score{ 0 },
	m_ScoreChangedEvent{ std::make_unique<Event>() }
{
}

void dae::ScoreComponent::AddScore(int amount)
{
	int currentScore{ m_Score };
	int newScore{ m_Score + amount };
	m_Score = newScore;

	if (newScore != currentScore) {
		m_ScoreChangedEvent->NotifyObservers(EventType::ScoreChanged,GetOwner());
	}
}

void dae::ScoreComponent::Notify(EventType eventType, GameObject* /*gameObject*/)
{
	switch (eventType)
	{
		case dae::EventType::EnemyDied:
			AddScore(100);
			break;
	}
}
