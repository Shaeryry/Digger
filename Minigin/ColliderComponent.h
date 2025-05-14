#pragma once
#include "Component.h"
#include "Event.h"
#include <memory>
#include <glm.hpp>
#include "PhysicsManager.h"

namespace Rinigin {
	class ColliderComponent final : public Component
	{ 
	public: 
		explicit ColliderComponent(GameObject* gameObject,const glm::vec3& size,const glm::vec3& offset,bool isTrigger);
		virtual ~ColliderComponent();

		Event* ColliderEnterEvent() const { return m_ColliderEnterEvent.get(); }
		Event* ColliderExitEvent() const { return m_ColliderExitEvent.get(); }

		glm::vec3 GetPosition() const;

		glm::vec3 Offset() const { return m_Offset; };
		glm::vec3 Bounds() const { return m_Bounds; };

		bool IsTrigger() const { return m_IsTrigger; };
		bool IsColliding(ColliderComponent* other);

		void SetOffset(const glm::vec3& offset) { m_Offset = offset; };
		void SetBounds(const glm::vec3& bounds) { m_Bounds = bounds; };
	private:
		friend void Physics::FixedUpdate();

		void AddCollidingCollider(ColliderComponent* other);
		void RemoveCollidingCollider(ColliderComponent* other);

		std::vector<ColliderComponent*> m_CollidingColliders;

		std::unique_ptr<Event> m_ColliderEnterEvent;
		std::unique_ptr<Event> m_ColliderExitEvent;

		bool m_IsTrigger;
		glm::vec3 m_Offset;
		glm::vec3 m_Bounds;
	};
}


