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

#include <iostream>

MoneyBagFalling::MoneyBagFalling(Rinigin::StateContextComponent* context, MoneyBag* moneyBag) :
	State(context),
	m_MoneyBag(moneyBag),
	m_FallHeight(0),
	m_Falling(false)
{
}

void MoneyBagFalling::Enter()
{  
	/*const glm::vec3 offset{ m_MoneyBag->GetCollider()->GetHalfExtents() };
	const glm::vec3 position{ m_MoneyBag->GetItemObject()->GetWorldPosition() + offset };
	m_MoneyBag->GetTerrain()->DigAt(static_cast<int>(position.x), static_cast<int>(position.y),25);*/
	m_Falling = false;
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
}
