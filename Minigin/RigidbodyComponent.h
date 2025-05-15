#pragma once
#include "Component.h"
#include <glm.hpp>

namespace Rinigin {
	class ColliderComponent;
	class RigidbodyComponent : public Component {
	public:
		explicit RigidbodyComponent(GameObject* gameObject, ColliderComponent* collider , float mass = 1.0f, bool isKinematic = false);
		virtual void FixedUpdate() override;

		glm::vec3 Velocity() const { return m_Velocity; }
		void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
		void SetMass(const float mass) { m_Mass = mass; };

		void ApplyImpulse(const glm::vec3& impulse);

		float Mass() const { return m_Mass; }
		bool IsKinematic() const { return m_IsKinematic; }

		ColliderComponent* GetCollider() const { return m_Collider; }
	private:
		ColliderComponent* m_Collider;

		glm::vec3 m_Velocity;
		float m_Mass;
		bool m_IsKinematic;
	};
}
