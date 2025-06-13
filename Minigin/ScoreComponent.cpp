#include "ScoreComponent.h"
#include "EventTypes.h"
#include "Helpers.h"
#include <iostream>
#include "ServiceLocator.h"
#include "DiggerConstants.h"

ScoreComponent::ScoreComponent(Rinigin::GameObject* gameObject) :
	Component(gameObject),
	m_Score( 0 ),
	m_ScoreChangedEvent(std::make_unique<Rinigin::Event>()),
	m_ScoreAddedEvent(std::make_unique<Rinigin::Event>())
{
}

void ScoreComponent::AddScore(int amount)
{
	if (amount <= 0) return;

	int currentScore{ m_Score };
	int newScore{ (m_Score + amount) };
	m_Score = newScore;

	if (newScore != currentScore) {
		std::cout << newScore << std::endl;

		ScoreArguments scoreAddedArgs{ "ScoreAdded",amount };
		m_ScoreAddedEvent->NotifyObservers(scoreAddedArgs);

		GameObjectEventArguments arguments{ "ScoreChanged",GetOwner() };
		m_ScoreChangedEvent->NotifyObservers(arguments);
	}
}

void ScoreComponent::Notify(Rinigin::EventArguments& eventArguments)
{
	switch (eventArguments.GetID())
	{
		case Rinigin::Helpers::sdbm_hash("EnemyDied"):
			AddScore(250);
			break;
		case Rinigin::Helpers::sdbm_hash("EmeraldCollected"): {
			GameObjectEventArguments& gameObjectArguments{ GetArgumentsOfType<GameObjectEventArguments>(eventArguments) };
			if (gameObjectArguments.GetGameObject() == GetOwner()) {
				Rinigin::ServiceLocator::GetSoundService().Play({ "ItemPickup.wav",DIGGER::SFX_VOLUME });
				AddScore(25);
			}
			break;
		}
		case Rinigin::Helpers::sdbm_hash("GoldCollected"): {

			GameObjectEventArguments& gameObjectArguments{ GetArgumentsOfType<GameObjectEventArguments>(eventArguments) };
			if (gameObjectArguments.GetGameObject() == GetOwner()) {
				Rinigin::ServiceLocator::GetSoundService().Play({ "Gold.wav",DIGGER::SFX_VOLUME });
				AddScore(500);
			}
			break;
		}
	}
	
}
