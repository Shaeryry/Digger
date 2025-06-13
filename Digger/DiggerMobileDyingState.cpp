#include "DiggerMobileDyingState.h" 
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include "Helpers.h"
#include "StateContextComponent.h"
#include "DiggerMobileDeadState.h"
#include "ServiceLocator.h"
#include "RigidbodyComponent.h"
#include <iostream>
#include "DiggerConstants.h"

DiggerMobileDyingState::DiggerMobileDyingState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DiggerMobile(digger) 
{  
}

void DiggerMobileDyingState::Enter()
{
	Rinigin::ServiceLocator::GetSoundService().Play({ "Damage.wav", DIGGER::SFX_VOLUME });

	m_DiggerMobile->GetRigidbody()->GravityEnabled(true);
	m_DiggerMobile->GetAnimator()->PlayAnimation("Dying");
	m_DiggerMobile->SetSpeed(0);
}

Rinigin::State* DiggerMobileDyingState::Update()
{
	if (glm::length(m_DiggerMobile->GetRigidbody()->Velocity()) <= 0) {
		return m_DiggerMobile->GetDeadState();
	}
	
	return nullptr;
}	

void DiggerMobileDyingState::Exit()
{
	m_DiggerMobile->GetRigidbody()->GravityEnabled(false);
}

