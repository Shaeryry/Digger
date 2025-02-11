#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae {
	class TransformComponent final : public Component
	{
	public:
		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z); 
	private:
		glm::vec3 m_position;
	};
}

