#include "MoneyBagIdle.h"
#include "MoneyBag.h"
#include "SpriteAnimatorComponent.h"
#include "ColliderComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "StateContextComponent.h"
#include <iostream>
#include "MoneyBagPrefall.h"

MoneyBagIdle::MoneyBagIdle(Rinigin::StateContextComponent* context, MoneyBag* moneyBag) :
	State(context),
	m_MoneyBag(moneyBag)
{
}

void MoneyBagIdle::Enter()
{ 
	m_MoneyBag->GetCollider()->RemoveExcludedLayer("Player");
	m_MoneyBag->GetCollider()->RemoveExcludedLayer("Nobbin");
	m_MoneyBag->GetAnimator()->PlayAnimation("Idle");
}

Rinigin::State* MoneyBagIdle::Update()
{
	const glm::vec3 boundsMoneyBag{ m_MoneyBag->GetCollider()->Bounds() };
	const glm::vec3 offsetPosition{ glm::vec3(0,boundsMoneyBag.y,0) + m_Offset };
	const glm::vec3 position{ m_MoneyBag->GetItemObject()->GetWorldPosition() + offsetPosition };

	float filledSpace{ Rinigin::Physics::GetInstance().GetMaskCoverage(position,(boundsMoneyBag * m_BoundsOffset)) };
	if (filledSpace < 0.5f) {
		return m_MoneyBag->GetStateMachine()->GetState<MoneyBagPrefall>();
	}
	return nullptr;
}

void MoneyBagIdle::Exit()
{
}
 