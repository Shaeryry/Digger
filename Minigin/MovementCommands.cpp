#include "MovementCommands.h"
#include "GameObject.h"
#include "Timer.h"


void dae::MovementCommand::MoveTo(const glm::vec3& direction)
{
	const glm::vec3& currentPosition{ GetGameObject()->GetWorldPosition() };
	const float movementSpeed{ m_Speed * Timer::GetInstance().deltaTime };

	GetGameObject()->SetPosition(currentPosition + (direction * movementSpeed) );
}

void dae::MoveUpCommand::Execute()
{
	const glm::vec3 direction{ 0, -1 ,0 };
	MoveTo(direction);
}

void dae::MoveDownCommand::Execute()
{
	const glm::vec3 direction{ 0, 1 ,0 };
	MoveTo(direction);
}

void dae::MoveLeftCommand::Execute()
{
	const glm::vec3 direction{ -1, 0 ,0 };
	MoveTo(direction);
}

void dae::MoveRightCommand::Execute()
{
	const glm::vec3 direction{ 1, 0 ,0 };
	MoveTo(direction);
}
