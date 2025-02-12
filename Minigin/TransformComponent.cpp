#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(GameObject* gameObject) : 
	Component(gameObject)
{
}

void dae::TransformComponent::SetPosition(const glm::vec2& position)
{
	SetPosition(position.x, position.y, 0);
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	SetPosition(position.x, position.y, position.z);
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;	
	m_Position.y = y;
	m_Position.z = z;
}
