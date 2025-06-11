#include "Emerald.h"
#include "SpriteSheetComponent.h"
#include "ColliderComponent.h"
#include "TextureComponent.h"
#include "Helpers.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Scene.h"

#include <iostream>
#include "Event.h"
#include "GameObject.h"
#include "EventTypes.h"

#include "Level.h"
#include "ScoreComponent.h"
#include "Character.h"

Emerald::Emerald(Level* level) :
	Item(level,"Emerald.png"),
	m_CollectEvent(std::make_unique<Rinigin::Event>()),
	m_Trigger(nullptr)
{
	m_Trigger = GetItemObject()->AddComponent<Rinigin::ColliderComponent>(glm::vec3{32,20,0}, glm::vec3{0,0,0}, true);
	m_Trigger->ColliderEnterEvent()->AddObserver(this);
}

void Emerald::Notify(Rinigin::EventArguments& eventArguments)
{
	switch (eventArguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("CollisionEnter"): {
		Rinigin::CollisionEventArguments& collisionArgument{ GetArgumentsOfType<Rinigin::CollisionEventArguments>(eventArguments) };
		const unsigned int layerId{ collisionArgument.GetOther()->GetCollisionLayer() };
		const unsigned int playerLayerId{ Rinigin::Helpers::sdbm_hash("Player") };
		if (layerId == playerLayerId) {
			GameObjectEventArguments arguments{ "EmeraldCollected",collisionArgument.GetOther()->GetOwner() };
			m_CollectEvent->NotifyObservers(arguments);
			GetItemObject()->Destroy(); // Remove the emerald !
			m_Level->GetEmeraldSpawner().RemoveTracked(this);
		};

		break;
	}
	default:
		break;
	}
}

Emerald* Emerald::Clone()
{
	Emerald* newEmerald = new Emerald(m_Level);
	newEmerald->GetItemObject()->SetActive(true);

	for (Character* character : m_Level->GetPlayers()) {
		ScoreComponent* scoreComponent = character->GetCharacterObject()->GetComponent<ScoreComponent>();
		if (scoreComponent) {
			newEmerald->GetCollectedEvent()->AddObserver(scoreComponent);
		}
	}
	return newEmerald;
}
