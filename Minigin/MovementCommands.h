#pragma once
#include "Command.h"
#include "glm.hpp"

namespace dae {
	class GameObject;
	class MovementCommand : public Command {
	public:
		explicit MovementCommand(GameObject* gameObject) : m_GameObject{ gameObject }, m_Speed{ 5.f } {};
		void SetSpeed(float speed) { m_Speed = speed; }
	protected:
		void MoveTo(const glm::vec3& direction);
		GameObject* GetGameObject() const { return m_GameObject; };
	private:
		GameObject* m_GameObject;
		float m_Speed;
	};

	class MoveUpCommand final : public MovementCommand {
	public:
		explicit MoveUpCommand(GameObject* gameObject) : MovementCommand(gameObject) {}
		virtual void Execute() override;
	};

	class MoveDownCommand final : public MovementCommand {
	public:
		explicit MoveDownCommand(GameObject* gameObject) : MovementCommand(gameObject) {}
		virtual void Execute() override;
	};

	class MoveLeftCommand final : public MovementCommand {
	public:
		explicit MoveLeftCommand(GameObject* gameObject) : MovementCommand(gameObject) {}
		virtual void Execute() override;
	};

	class MoveRightCommand final : public MovementCommand {
	public:
		explicit MoveRightCommand(GameObject* gameObject) : MovementCommand(gameObject) {}
		virtual void Execute() override;
	};

}

