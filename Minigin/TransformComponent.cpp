#include "TransformComponent.h"

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	SetPosition(position.x, position.y, position.z);
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
