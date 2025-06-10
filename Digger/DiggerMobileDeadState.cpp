#include "DiggerMobileDeadState.h"
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include "RigidbodyComponent.h"
#include "LivesComponent.h"
#include "Timer.h"
#include "Level.h"
#include "DiggerMobileDiggingState.h"
#include "DiggerConstants.h"

DiggerMobileDeadState::DiggerMobileDeadState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DiggerMobile(digger),
	m_DeadClock(0)
{
}

void DiggerMobileDeadState::Enter()
{
	m_DeadClock = 0;

	m_DiggerMobile->GetRigidbody()->SetCanCollide(false);
	m_DiggerMobile->GetAnimator()->PlayAnimation("Dead");
	m_DiggerMobile->SetSpeed(0);

	m_DiggerMobile->GetLevel()->AddDeadPlayer(m_DiggerMobile);
	m_DiggerMobile->GetLivesComponent()->RemoveLives(1);
}


Rinigin::State* DiggerMobileDeadState::Update()
{
	if (m_DiggerMobile->GetLivesComponent()->GetLives() > 0) {
		if (m_DeadClock > DIGGER::RESPAWN_TIME) {
			m_DiggerMobile->GetLevel()->RespawnPlayer(m_DiggerMobile->GetDiggerIndex(), false);
			return m_DiggerMobile->GetDiggingState();
		}
	}
	m_DeadClock += Rinigin::Timer::GetInstance().deltaTime;
	return nullptr;
}

void DiggerMobileDeadState::Exit()
{
}
