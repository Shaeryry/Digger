#include "MovementCommands.h"
#include "GameObject.h"
#include "Timer.h"

void MovementCommand::Execute()
{
	const glm::vec3& currentPosition{ m_GameObject->GetWorldPosition() };
	const float movementSpeed{ m_Speed * Rinigin::Timer::GetInstance().deltaTime };

	m_GameObject->SetPosition(currentPosition + (m_Direction * movementSpeed));
} 