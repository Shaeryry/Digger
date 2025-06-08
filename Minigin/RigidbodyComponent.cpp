#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "Timer.h"
#include "ColliderComponent.h"

#include <iostream>

Rinigin::RigidbodyComponent::RigidbodyComponent(GameObject* gameObject, ColliderComponent* collider, float mass, bool isKinematic) :
	Component(gameObject),
	m_Collider(collider),
	m_Velocity(0, 0, 0),
	m_Force(0, 0, 0),
	m_Mass(mass),
	m_IsKinematic(isKinematic),
	m_UseGravity(true)
{
	Rinigin::Physics::GetInstance().AddRigidbody(this);
}

void Rinigin::RigidbodyComponent::FixedUpdate()
{ 
	if (m_IsKinematic) return;

	auto& physics = Rinigin::Physics::GetInstance();
	const float dt = Timer::GetInstance().deltaTime;
	const glm::vec3 position = GetOwner()->GetWorldPosition();

	if (m_UseGravity) AddForce(Rinigin::Physics::GetInstance().Gravity() * m_Mass);

	// Apply force-based acceleration
	glm::vec3 acceleration = (m_Force / m_Mass);
	m_Velocity += acceleration * dt;

	// Apply velocity to position
	glm::vec3 velocity = (m_Velocity * dt);
	glm::vec3 nextPosition = (position + velocity);

	// Prevent movement if you're colliding with a collision mask
	bool nextPositionSolid{ physics.IsOverlappingWithMasks(nextPosition,m_Collider->Bounds()) };
	if (nextPositionSolid) { 
		m_Force = glm::vec3{ 0,0,0 }; // Reset force
		SetVelocity( glm::vec3{ 0,0,0 } );
		nextPosition = position;
	};

	GetOwner()->SetPosition(nextPosition);
	m_Force = glm::vec3{0,0,0}; // Reset force
}

void Rinigin::RigidbodyComponent::AddForce(const glm::vec3& force)
{
	m_Force += force;
}
