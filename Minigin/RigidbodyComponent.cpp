#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "Timer.h"

Rinigin::RigidbodyComponent::RigidbodyComponent(GameObject* gameObject, ColliderComponent* collider, float mass, bool isKinematic) :
	Component(gameObject),
	m_Collider(collider),
	m_Velocity(0, 0, 0),
	m_Mass(mass),
	m_IsKinematic(isKinematic)
{
	Rinigin::Physics::GetInstance().AddRigidbody(this);
}

void Rinigin::RigidbodyComponent::FixedUpdate()
{
	if (!m_IsKinematic)
	{
		float dt = Timer::GetInstance().deltaTime;
		auto pos = GetOwner()->GetWorldPosition();
		GetOwner()->SetPosition(pos + m_Velocity * dt);
	}
}

void Rinigin::RigidbodyComponent::ApplyImpulse(const glm::vec3& impulse)
{
	if (!m_IsKinematic and m_Mass > 0.0f) m_Velocity += impulse / m_Mass;
}
