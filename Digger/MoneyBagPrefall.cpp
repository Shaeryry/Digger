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
#include "ServiceLocator.h"
#include "DiggerConstants.h"

#include <iostream>

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
	Rinigin::ServiceLocator::GetSoundService().Play({ "BagWiggle.wav", DIGGER::SFX_VOLUME });
}

Rinigin::State* MoneyBagPrefall::Update()
{
	float velocity = glm::length(m_MoneyBag->GetRigidbody()->Velocity());
	m_Duration += Rinigin::Timer::GetInstance().deltaTime;

	if (m_Duration > DIGGER::BAG_WIGGLE_TIME or velocity > 0) {
		return m_MoneyBag->GetStateMachine()->GetState<MoneyBagFalling>();
	}
	return nullptr;
}

void MoneyBagPrefall::Exit()
{
	Rinigin::ServiceLocator::GetSoundService().Stop({ "BagWiggle.wav" });
}
