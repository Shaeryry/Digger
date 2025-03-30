#include "ScoreComponent.h"
#include "EventTypes.h"
#include "Helpers.h"

ScoreComponent::ScoreComponent(Rinigin::GameObject* gameObject) :
	Component(gameObject),
	m_Score{ 0 },
	m_ScoreChangedEvent{ std::make_unique<Rinigin::Event>( GameObjectEventArguments("ScoreChanged",gameObject))}
{
}

void ScoreComponent::AddScore(int amount)
{
	int currentScore{ m_Score };
	int newScore{ m_Score + amount };
	m_Score = newScore;

	if (newScore != currentScore) {
		m_ScoreChangedEvent->NotifyObservers();
	}
}

void ScoreComponent::Notify(Rinigin::EventArguments* eventArgs)
{
	switch (eventArgs->GetID())
	{
		case Rinigin::Helpers::sdbm_hash("EnemyDied"):
			AddScore(100);
			break;
	}
}
