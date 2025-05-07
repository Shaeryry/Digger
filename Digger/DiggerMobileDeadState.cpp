#include "DiggerMobileDeadState.h"
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include <iostream>

DiggerMobileDeadState::DiggerMobileDeadState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DiggerMobile(digger)
{
}

void DiggerMobileDeadState::Enter()
{
	std::cout << "dead" << std::endl;

	m_DiggerMobile->GetAnimator()->PlayAnimation("Dead");
	m_DiggerMobile->SetSpeed(0);
}

void DiggerMobileDeadState::Update()
{
}

void DiggerMobileDeadState::Exit()
{
}
