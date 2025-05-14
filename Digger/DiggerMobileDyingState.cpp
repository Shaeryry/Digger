#include "DiggerMobileDyingState.h" 
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include "Helpers.h"
#include "StateContextComponent.h"
#include "DiggerMobileDeadState.h"
#include "ServiceLocator.h"

#include <iostream>

DiggerMobileDyingState::DiggerMobileDyingState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DiggerMobile(digger) 
{  
}

void DiggerMobileDyingState::Enter()
{
	std::cout << "dying" << std::endl;
	Rinigin::ServiceLocator::GetSoundService().Play({ "bruh.wav", .25f });

	m_DiggerMobile->GetAnimator()->PlayAnimation("Dying");
	m_DiggerMobile->GetAnimator()->GetEndedEvent()->AddObserver(this);

	m_DiggerMobile->SetSpeed(0);
}

void DiggerMobileDyingState::Update()
{
}	

void DiggerMobileDyingState::Exit()
{
	m_DiggerMobile->GetAnimator()->GetEndedEvent()->RemoveObserver(this);
}

// TEMPORARY TO TRANSITION TO DEAD STATE

void DiggerMobileDyingState::Notify(Rinigin::EventArguments* arguments)
{
	switch (arguments->GetID())
	{
	case Rinigin::Helpers::sdbm_hash("AnimationEnded"):
		GetContext()->SetState( m_DiggerMobile->GetDeadState() );
		break;
	default:
		break;
	}
}

