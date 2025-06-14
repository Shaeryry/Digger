#pragma once
#include <State.h>

class MoneyBag;
class MoneyBagPrefall final : public Rinigin::State
{
public:
	explicit MoneyBagPrefall(Rinigin::StateContextComponent* context, MoneyBag* moneyBag);
	virtual void Enter();
	virtual Rinigin::State* Update(); 
	virtual void Exit();
private:
	MoneyBag* m_MoneyBag;
	float m_Duration;
};

