#pragma once
#include <State.h>
#include <glm/glm.hpp>

class MoneyBag;
class MoneyBagIdle final : public Rinigin::State
{
public:
	explicit MoneyBagIdle(Rinigin::StateContextComponent* context, MoneyBag* moneyBag);
	virtual void Enter();
	virtual Rinigin::State* Update();
	virtual void Exit();
private:
	MoneyBag* m_MoneyBag;
	glm::vec3 m_Offset = glm::vec3{0,10.f,0};
	float m_BoundsOffset = 0.5f; // Multiplier for the bounding box
};

