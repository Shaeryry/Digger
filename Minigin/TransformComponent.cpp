#include "TransformComponent.h"
#include "GameObject.h"

Rinigin::TransformComponent::TransformComponent(GameObject* gameObject) :
	Component(gameObject),
	m_DirtyPosition{ true },
	m_WorldPosition{ 0,0,0 },
	m_LocalPosition{ 0,0,0 }
{
}

void Rinigin::TransformComponent::MakeDirty()
{
	MakePositionDirty();
}

void Rinigin::TransformComponent::SetLocalPosition(const glm::vec2& position)
{
	SetLocalPosition(position.x, position.y, 0);
}

void Rinigin::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	SetLocalPosition(position.x, position.y, position.z);
}

void Rinigin::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	MakePositionDirty();
	GetOwner()->NotifyPositionChanged(); // Mark all the children for update !
}


void Rinigin::TransformComponent::SetWorldPosition(const glm::vec2& position)
{
	SetWorldPosition(position.x, position.y, 0);
}

void Rinigin::TransformComponent::SetWorldPosition(const glm::vec3& position)
{
	SetWorldPosition(position.x, position.y, position.z);
}

void Rinigin::TransformComponent::SetWorldPosition(float x, float y, float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
}

void Rinigin::TransformComponent::UpdateWorldPosition()
{
	if (m_DirtyPosition) {
		GameObject* parent{ GetOwner()->GetParent() };

		if (parent) {
			SetWorldPosition(parent->GetWorldPosition() + GetTransformLocalPosition());
		}
		else {
			SetWorldPosition(GetTransformLocalPosition());
		}

		//GetOwner()->NotifyWorldPositionChanged(); // Mark all the children for update !
		m_DirtyPosition = false;
	}
}
