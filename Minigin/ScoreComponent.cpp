#include "ScoreComponent.h"
#include "EventTypes.h"
#include "Helpers.h"
#include <iostream>

ScoreComponent::ScoreComponent(Rinigin::GameObject* gameObject) :
	Component(gameObject),
	m_Score{ 0 },
	m_ScoreChangedEvent{ std::make_unique<Rinigin::Event>()}
{
}

void ScoreComponent::AddScore(int amount)
{
	int currentScore{ m_Score };
	int newScore{ m_Score + amount };
	m_Score = newScore;

	if (newScore != currentScore) {
		std::cout << newScore << std::endl;
		GameObjectEventArguments arguments{ "ScoreChanged",GetOwner() };
		m_ScoreChangedEvent->NotifyObservers(arguments);
	}
}

void ScoreComponent::Notify(Rinigin::EventArguments& eventArguments)
{
	switch (eventArguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("EnemyDied"):
		AddScore(100);
		break;
	case Rinigin::Helpers::sdbm_hash("EmeraldCollected"): {
		//AddScore(100);
		GameObjectEventArguments& gameObjectArguments{ GetArgumentsOfType<GameObjectEventArguments>(eventArguments) };
		if (gameObjectArguments.GetGameObject() == GetOwner()) {
			AddScore(100);
			break;
		}
	}
	}
	
}
