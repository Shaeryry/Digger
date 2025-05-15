#pragma once
#include "Singleton.h"
#include "Event.h"
#include <vector>

namespace Rinigin {
	class ColliderComponent;
	class RigidbodyComponent;

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

		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);

		void AddRigidbody(RigidbodyComponent* rigidbody);
		void RemoveRigidbody(RigidbodyComponent* rigidbody);
	private:
		bool AreCollidersOverlapping(ColliderComponent* collider_A, ColliderComponent* collider_B);

		std::vector<ColliderComponent*> m_Colliders;
		std::vector<RigidbodyComponent*> m_Rigidbodies;
	};
}

