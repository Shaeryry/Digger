#include "MoneyBagPrefall.h"
#include "SpriteAnimatorComponent.h"
#include "ColliderComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "StateContextComponent.h"
#include "MoneyBag.h"

#include "MoneyBagFalling.h"
#include "Timer.h"
#include "RigidbodyComponent.h"

MoneyBagPrefall::MoneyBagPrefall(Rinigin::StateContextComponent* context, MoneyBag* moneyBag) : 
	State(context),
	m_Duration(0),
	m_MoneyBag(moneyBag)
{
}

void MoneyBagPrefall::Enter()
{
	m_Duration = 0;
	m_MoneyBag->GetAnimator()->PlayAnimation("Prefall");
}

Rinigin::State* MoneyBagPrefall::Update()
{
	float velocity = glm::length(m_MoneyBag->GetRigidbody()->Velocity());
	m_Duration += Rinigin::Timer::GetInstance().deltaTime;

	if (m_Duration > m_FallTime or velocity > 0) {
		return m_MoneyBag->GetStateMachine()->GetState<MoneyBagFalling>();
	}
	return nullptr;
}

void MoneyBagPrefall::Exit()
{
}
