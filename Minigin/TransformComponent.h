#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae {
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* gameObject);
		const glm::vec3& GetTransformWorldPosition() const { return m_WorldPosition; };
		const glm::vec3& GetTransformLocalPosition() const { return m_LocalPosition; };

		void SetWorldPosition(const glm::vec2& position);
		void SetWorldPosition(const glm::vec3& position);
		void SetWorldPosition(float x, float y, float z); 

		void SetLocalPosition(const glm::vec2& position);
		void SetLocalPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z);
	private:
		glm::vec3 m_WorldPosition;
		glm::vec3 m_LocalPosition;
	};
}

