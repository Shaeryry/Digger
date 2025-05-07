#include "MovementCommands.h"
#include "GameObject.h"
#include "Timer.h"

void MovementCommand::Execute()
{
	const glm::vec3& currentPosition{ m_GameObject->GetWorldPosition() };
	const glm::vec3 invertedDirection{ m_Direction.x,-m_Direction.y,m_Direction.z };

	const float movementSpeed{ m_Speed * Rinigin::Timer::GetInstance().deltaTime };
	const glm::vec3 displacement{ invertedDirection * movementSpeed };

	m_GameObject->SetPosition(currentPosition + displacement);
}

ChangeDirectionCommand::ChangeDirectionCommand(MovementCommand* movementCommand, const glm::vec3& direction) : 
	m_DirectionChangedEvent(std::make_unique<Rinigin::Event>(Rinigin::NullEventArguments("DirectionChanged"))),
	m_Movement(movementCommand), 
	m_Direction(direction)
{
}

void ChangeDirectionCommand::Execute()
{
	if (m_Movement) {
		m_Movement->SetDirection(m_Direction);
		m_DirectionChangedEvent->NotifyObservers();
	}
}
