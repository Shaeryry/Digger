#include "DiggerMobileDiggingState.h"
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include "MovementCommands.h"
#include "Helpers.h"
#include "Timer.h"
#include "StateContextComponent.h"
#include "DiggerMobileDyingState.h"

DiggerMobileDiggingState::DiggerMobileDiggingState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DirectionName("Right"),
	m_DiggerMobile(digger)
{

}

void DiggerMobileDiggingState::Enter()
{
	m_DiggerMobile->UpDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->DownDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->RightDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->LeftDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);

	m_DiggerMobile->SetSpeed(m_Speed);
	UpdateAnimation();
}

void DiggerMobileDiggingState::Update()
{
	GetContext()->SetState( m_DiggerMobile->GetDyingState() );
} 

void DiggerMobileDiggingState::Exit()
{
	m_DiggerMobile->UpDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->DownDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->RightDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->LeftDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
}

void DiggerMobileDiggingState::Notify(Rinigin::EventArguments* arguments)
{
	switch (arguments->GetID())
	{
	case Rinigin::Helpers::sdbm_hash("DirectionChanged"):

		glm::vec3 currentDirection = m_DiggerMobile->GetMoveCommand()->GetDirection();
		if (currentDirection == glm::vec3(0, 1, 0)) m_DirectionName = "Up";
		if (currentDirection == glm::vec3(0, -1, 0)) m_DirectionName = "Down";
		if (currentDirection == glm::vec3(1, 0, 0)) m_DirectionName = "Right";
		if (currentDirection == glm::vec3(-1, 0, 0)) m_DirectionName = "Left";
		
		UpdateAnimation();
		break;
	default:
		break;
	}
}

void DiggerMobileDiggingState::UpdateAnimation()
{
	m_DiggerMobile->GetAnimator()->PlayAnimation(m_DirectionName);  // Update the animation
}
