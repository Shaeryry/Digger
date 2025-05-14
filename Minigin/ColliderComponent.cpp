#include "ColliderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsManager.h"
#include <iostream>

Rinigin::ColliderComponent::ColliderComponent(GameObject* gameObject, const glm::vec3& size, const glm::vec3& offset, bool isTrigger) :
	Component(gameObject),

	m_ColliderEnterEvent(),
	m_ColliderExitEvent(),
	m_IsTrigger(isTrigger),

	m_Bounds(size),
	m_Offset(offset)
{
	Physics::GetInstance().AddCollider(this);
}

Rinigin::ColliderComponent::~ColliderComponent()
{
	Physics::GetInstance().RemoveCollider(this);
}

glm::vec3 Rinigin::ColliderComponent::GetPosition() const
{
	glm::vec3 origin{ GetOwner()->GetWorldPosition() };
	return (origin + m_Offset);
}

void Rinigin::ColliderComponent::AddCollidingCollider(ColliderComponent* other)
{
	if ( IsColliding(other) ) return;
	m_CollidingColliders.emplace_back(other);
	std::cout << "did collision" << std::endl;
	//m_ColliderEnterEvent->NotifyObservers();
}

void Rinigin::ColliderComponent::RemoveCollidingCollider(ColliderComponent* other)
{	
	if (m_CollidingColliders.empty()) return;
	m_CollidingColliders.erase(std::remove(m_CollidingColliders.begin(), m_CollidingColliders.end(), other), m_CollidingColliders.end());
	std::cout << "lost collision" << std::endl;

	//m_ColliderExitEvent->NotifyObservers();
}

bool Rinigin::ColliderComponent::IsColliding(ColliderComponent* other)
{
	auto foundIt = std::find(m_CollidingColliders.begin(), m_CollidingColliders.end(), other);
	return (foundIt != m_CollidingColliders.end());
}
