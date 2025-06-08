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

		virtual void FixedUpdate() override;

		Event* ColliderEnterEvent() const { return m_ColliderEnterEvent.get(); }
		Event* ColliderExitEvent() const { return m_ColliderExitEvent.get(); }

		glm::vec3 GetPosition() const;
		glm::vec3 GetCenter() const;
		glm::vec3 GetHalfExtents() const;
		glm::vec3 Offset() const { return m_Offset; };
		glm::vec3 Bounds() const { return m_Bounds; };

		bool IsTrigger() const { return m_IsTrigger; };
		bool IsTouching(ColliderComponent* other);

		void SetOffset(const glm::vec3& offset) { m_Offset = offset; };
		void SetBounds(const glm::vec3& bounds) { m_Bounds = bounds; };

		void SetLayer(const char* layerName);
		void SetLayer(unsigned int id) { m_LayerId = id; };
		unsigned int GetCollisionLayer() const { return m_LayerId; };

		void AddExcludedLayer(const char* layerName);
		void AddExcludedLayer(unsigned int layerId);
		void RemoveExcludedLayer(unsigned int layerId);
		bool IsLayerExcluded(unsigned int layerId);

		size_t GetCollisions() const { return m_CollidingColliders.size(); };
	private:
		friend void Physics::DetectCollisions();
		friend void Physics::RemoveCollider(ColliderComponent* collider);

		void AddCollidingCollider(ColliderComponent* other);
		void RemoveCollidingCollider(ColliderComponent* other);

		unsigned int m_LayerId;
		std::vector<unsigned int> m_ExcludedLayers;

		std::vector<ColliderComponent*> m_CollidingColliders;

		std::unique_ptr<Event> m_ColliderEnterEvent;
		std::unique_ptr<Event> m_ColliderExitEvent;

		bool m_IsTrigger;

		glm::vec3 m_Offset;
		glm::vec3 m_Bounds;
	};
}


