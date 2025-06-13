#pragma once
#include "Command.h"
#include "Event.h"
#include "glm.hpp"

namespace Rinigin { class GameObject; };
class MovementCommand final : public Rinigin::Command {
public:
	explicit MovementCommand(Rinigin::GameObject* gameObject,const glm::vec3& direction,const float speed) 
		: m_GameObject{ gameObject }, m_Speed{ speed }, m_Direction{ direction } {
		SetMaxInputs(1);
	};

	void SetSpeed(float speed) { m_Speed = speed; }
	void SetDirection(const glm::vec3& direction) { m_Direction = direction; };
	glm::vec3 GetDirection() const { return m_Direction; }
	virtual void Execute() override; 
private: 
	Rinigin::GameObject* m_GameObject;
	glm::vec3 m_Direction;
	float m_Speed;
};

class ChangeDirectionCommand final : public Rinigin::Command {
	public: 
		explicit ChangeDirectionCommand(MovementCommand* movementCommand, const glm::vec3& direction);
		virtual void Execute() override;
		Rinigin::Event* GetDirectionChangedEvent() const { return m_DirectionChangedEvent.get(); };
	private:
		std::unique_ptr<Rinigin::Event> m_DirectionChangedEvent;
		MovementCommand* m_Movement;
		glm::vec3 m_Direction;
};

