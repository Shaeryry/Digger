#include "DiggerMobileDiggingState.h"
#include "DiggerMobile.h"
#include "SpriteAnimatorComponent.h"
#include "MovementCommands.h"
#include "Helpers.h"
#include "Timer.h"
#include "StateContextComponent.h"
#include "DiggerMobileDyingState.h"
#include "TerrainComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteSheetComponent.h"
#include "DiggerConstants.h"

#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Level.h"
#include "PhysicsManager.h"
#include "DiggerMobile.h"
#include "HealthComponent.h"

DiggerMobileDiggingState::DiggerMobileDiggingState(Rinigin::StateContextComponent* context, DiggerMobile* digger) :
	State(context),
	m_DirectionName("Right"),
	m_DiggerMobile(digger)
{

}

void DiggerMobileDiggingState::Enter()
{
	m_DiggerMobile->GetRigidbody()->SetCanCollide(true);

	m_DiggerMobile->UpDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->DownDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->RightDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);
	m_DiggerMobile->LeftDirectionCommand()->GetDirectionChangedEvent()->AddObserver(this);

	m_DiggerMobile->SetSpeed(DIGGER::DIGGER_SPEED);
	m_DiggerMobile->GetTrigger()->ColliderEnterEvent()->AddObserver(this);

	UpdateAnimation();
}

Rinigin::State* DiggerMobileDiggingState::Update()
{
	// DIG DIG DIG
	TerrainComponent* map{ m_DiggerMobile->GetLevel()->Map() };
	if (map) {
		glm::vec3 diggerMobilePosition{ m_DiggerMobile->GetCharacterObject()->GetTransform()->GetTransformLocalPosition() };
		float xPos{ diggerMobilePosition.x + (m_DiggerMobile->GetSpriteSheetComponent()->GetTileWidth() / 2) };
		float yPos{ diggerMobilePosition.y + (m_DiggerMobile->GetSpriteSheetComponent()->GetTileHeight() / 2) };

		map->DigAt(xPos, yPos, DIGGER::DIG_TUNNEL_SIZE);
	}

	return nullptr;
} 

void DiggerMobileDiggingState::Exit()
{
	m_DiggerMobile->GetTrigger()->ColliderEnterEvent()->RemoveObserver(this);

	m_DiggerMobile->UpDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->DownDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->RightDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
	m_DiggerMobile->LeftDirectionCommand()->GetDirectionChangedEvent()->RemoveObserver(this);
}

void DiggerMobileDiggingState::Notify(Rinigin::EventArguments & eventArguments)
{
	switch (eventArguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("DirectionChanged"):

		glm::vec3 currentDirection = m_DiggerMobile->GetMoveCommand()->GetDirection();
		if (currentDirection == glm::vec3(0, -1, 0)) m_DirectionName = "Up";
		if (currentDirection == glm::vec3(0, 1, 0)) m_DirectionName = "Down";
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
