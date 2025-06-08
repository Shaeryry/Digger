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

Emerald::Emerald() :
	Item("Emerald.png"),
	m_CollectEvent(std::make_unique<Rinigin::Event>())
{
	Rinigin::ColliderComponent* collider = GetTrigger();
	collider->SetBounds(glm::vec3{ 32,20,0 });
	collider->ColliderEnterEvent()->AddObserver(this);
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
		};

		break;
	}
	default:
		break;
	}
}
