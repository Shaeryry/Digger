#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae {
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* gameObject);
		const glm::vec3& GetPosition() const { return m_Position; };
		void SetPosition(const glm::vec2& position);
		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z); 
	private:
		glm::vec3 m_Position;
	};
}

