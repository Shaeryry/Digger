#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae {
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* gameObject);
		const glm::vec3& GetTransformWorldPosition() { UpdateWorldPosition(); return m_WorldPosition; };
		const glm::vec3& GetTransformLocalPosition() const { return m_LocalPosition; };

		void MakeDirty(); // Makes the component fully dirty and will mark everything for an update.
		void MakePositionDirty() { m_DirtyPosition = true; };

		void SetLocalPosition(const glm::vec2& position);
		void SetLocalPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);
	private:	
		void SetWorldPosition(const glm::vec2& position);
		void SetWorldPosition(const glm::vec3& position);
		void SetWorldPosition(float x, float y, float z);
		void UpdateWorldPosition();

		// Flags
		bool m_DirtyPosition;

		// Data
		glm::vec3 m_WorldPosition;
		glm::vec3 m_LocalPosition;
	};
}

