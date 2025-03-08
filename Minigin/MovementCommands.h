#pragma once
#include "Command.h"
#include "glm.hpp"

namespace dae {
	class GameObject;
	class MovementCommand final : public Command {
	public:
		explicit MovementCommand(GameObject* gameObject,const glm::vec3& direction,const float speed) 
			: m_GameObject{ gameObject }, m_Speed{ speed }, m_Direction{ direction } {};

		void SetSpeed(float speed) { m_Speed = speed; }
		void SetDirection(const glm::vec3& direction) { m_Direction = direction; };
		virtual void Execute() override;
	private:

		GameObject* m_GameObject;
		glm::vec3 m_Direction;
		float m_Speed;
	};
}

