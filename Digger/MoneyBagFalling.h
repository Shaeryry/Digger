#pragma once
#include <State.h>

class MoneyBag;
class MoneyBagFalling final : public Rinigin::State
{
public:
	explicit MoneyBagFalling(Rinigin::StateContextComponent* context, MoneyBag* moneyBag);
	virtual void Enter();
	virtual Rinigin::State* Update();
	virtual void Exit();
private:
	MoneyBag* m_MoneyBag;
	bool m_Falling;
	float m_FallHeight;
};