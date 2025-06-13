#include "MoneyBagFalling.h"
#include "SpriteAnimatorComponent.h"
#include "MoneyBag.h"
#include "TerrainComponent.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "PhysicsManager.h"
#include "MoneyBagIdle.h"
#include "StateContextComponent.h"
#include "Level.h"
#include "PrototypeSpawner.h"
#include "EventTypes.h"
#include <iostream>
#include "HealthComponent.h"
#include "ServiceLocator.h"
#include "DiggerConstants.h"

MoneyBagFalling::MoneyBagFalling(Rinigin::StateContextComponent* context, MoneyBag* moneyBag) :
	State(context),
	m_MoneyBag(moneyBag),
	m_FallHeight(0),
	m_Falling(false)
{
}

void MoneyBagFalling::Enter()
{  
	m_Falling = false;
	Rinigin::ServiceLocator::GetSoundService().Play({ "BagFalling.wav",DIGGER::SFX_VOLUME });

	m_MoneyBag->GetTrigger()->ColliderEnterEvent()->AddObserver(this);
	m_MoneyBag->GetRigidbody()->SetCanCollide(false);
	m_MoneyBag->GetAnimator()->PlayAnimation("Falling");

	m_FallHeight = m_MoneyBag->GetItemObject()->GetWorldPosition().y;
}

Rinigin::State* MoneyBagFalling::Update()
{
	const glm::vec3 position{ m_MoneyBag->GetItemObject()->GetWorldPosition() };
	float velocity = glm::length(m_MoneyBag->GetRigidbody()->Velocity());

	if (not m_Falling) {
		const glm::vec3 bounds{ m_MoneyBag->GetCollider()->Bounds() };
		const bool colliding = Rinigin::Physics::GetInstance().IsOverlappingWithMasks(position, bounds);

		if (not colliding) {
			m_Falling = true;
			m_MoneyBag->GetRigidbody()->SetCanCollide(true);
			m_MoneyBag->GetCollider()->AddExcludedLayer("Player");
		}

		return nullptr;
	}
	else {
		if (velocity <= 0) {
			float fallenDistance = (position.y - m_FallHeight);
			std::cout << fallenDistance << std::endl;
			if (fallenDistance > 60.f) {
				// Destroy and lowkey just die
				Item* gold = m_MoneyBag->GetLevel()->GetItemSpawner().Spawn("Gold");
				gold->GetItemObject()->SetPosition(m_MoneyBag->GetItemObject()->GetWorldPosition());
				EndSFX();
				m_MoneyBag->GetItemObject()->Destroy();
			}
			else {
				return m_MoneyBag->GetStateMachine()->GetState<MoneyBagIdle>();
			}
		}
	}

	return nullptr;
}

void MoneyBagFalling::Exit()
{
	EndSFX();
	m_MoneyBag->GetTrigger()->ColliderEnterEvent()->RemoveObserver(this);
}

void MoneyBagFalling::Notify(Rinigin::EventArguments& eventArguments)
{
	switch (eventArguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("CollisionEnter"): {
		Rinigin::CollisionEventArguments& collisionArgument{ GetArgumentsOfType<Rinigin::CollisionEventArguments>(eventArguments) };
		const unsigned int layerId{ collisionArgument.GetOther()->GetCollisionLayer() };
		const unsigned int playerLayerId{ Rinigin::Helpers::sdbm_hash("Player") };

		if (layerId == playerLayerId) {
			HealthComponent* healthComponent = collisionArgument.GetOther()->GetOwner()->GetComponent<HealthComponent>();
			// Check if the bag fell on your head
			if (healthComponent) {
				healthComponent->TakeDamage(1);
			}
		};

		break;
	}
	default:
		break;
	}
}

void MoneyBagFalling::EndSFX()
{
	Rinigin::ServiceLocator::GetSoundService().Play({ "BagFell.wav",DIGGER::SFX_VOLUME });
	Rinigin::ServiceLocator::GetSoundService().Stop({ "BagFalling.wav" });
}

