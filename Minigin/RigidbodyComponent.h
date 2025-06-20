#pragma once
#include "Component.h"
#include <glm.hpp>

namespace Rinigin {
	class ColliderComponent;
	class RigidbodyComponent final : public Component {
	public:
		explicit RigidbodyComponent(GameObject* gameObject, ColliderComponent* collider , float mass = 1.0f, bool isKinematic = false);
		~RigidbodyComponent();
		virtual void FixedUpdate() override;
		
		void SetKinematic(bool kinematic) { m_IsKinematic = kinematic; };
		void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
		void SetMass(const float mass) { m_Mass = mass; };
		void GravityEnabled(bool state) { m_UseGravity = state; }

		void AddForce(const glm::vec3& force);
	
		glm::vec3 Velocity() const { return m_Velocity; }

		float Mass() const { return m_Mass; }
		bool IsKinematic() const { return m_IsKinematic; }
		bool CanCollide() const { return m_CanCollide; }
		void SetCanCollide(bool state) { m_CanCollide = state; }
		 
		ColliderComponent* GetCollider() const { return m_Collider; }

		void SetAxisLock(float x, float y, float z) { m_AxisLock = glm::vec3(x, y, z); };
		glm::vec3 AxisLock() const { return m_AxisLock; }
	private:
		ColliderComponent* m_Collider;
			
		glm::vec3 m_Velocity;
		glm::vec3 m_Force;
		glm::vec3 m_AxisLock;


		float m_Mass;
		bool m_IsKinematic;
		bool m_UseGravity;
		bool m_CanCollide;
	};
}
