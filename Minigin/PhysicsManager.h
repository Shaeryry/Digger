#pragma once
#include "Singleton.h"
#include <vector>

namespace Rinigin {
	class ColliderComponent;

	class Physics : public Singleton<Physics>
	{
	public:
		void Render() const;
		void FixedUpdate();

		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);
	private:
		bool IsColliding(ColliderComponent* collider_A, ColliderComponent* collider_B);
		std::vector<ColliderComponent*> m_Colliders;
	};
}

