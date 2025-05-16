#pragma once
#include "Singleton.h"
#include "Event.h"
#include <vector>
#include <glm.hpp>

namespace Rinigin {
	class ColliderComponent;
	class RigidbodyComponent;
	class ICollisionMask;

	class CollisionEventArguments final : public EventArguments {
	public:
		explicit CollisionEventArguments(const char* eventName,ColliderComponent* collider) : EventArguments(eventName),m_Collider(collider) {};
		ColliderComponent* GetCollider() const { return m_Collider; };
	private:
		ColliderComponent* m_Collider;
	};

	class Physics : public Singleton<Physics>
	{
	public:
		void Render() const;
		void FixedUpdate();

		void DetectCollisions();
		void SolveCollisions();
		bool IsOverlappingWithMasks(glm::vec3 position,glm::vec3 bounds) const;

		void AddCollisionMask(ICollisionMask* mask);
		void RemoveCollisionMask(ICollisionMask* mask);

		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);

		void AddRigidbody(RigidbodyComponent* rigidbody);
		void RemoveRigidbody(RigidbodyComponent* rigidbody);

		void SetGravity(float gravity) { m_Gravity = gravity; };
		glm::vec3 Gravity() const { return {0,m_Gravity,0}; };
	private:
		bool AreCollidersOverlapping(ColliderComponent* collider_A, ColliderComponent* collider_B);
		bool IsSolid(int x, int y) const;

		std::vector<ColliderComponent*> m_Colliders;
		std::vector<RigidbodyComponent*> m_Rigidbodies;
		std::vector<ICollisionMask*> m_CollisionMasks;
		float m_Gravity = 9.81f;
	};
}

