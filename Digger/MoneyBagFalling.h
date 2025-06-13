#pragma once
#include <State.h>
#include "Observer.h"

class MoneyBag;
class MoneyBagFalling final : public Rinigin::State,public Rinigin::Observer
{
public:
	explicit MoneyBagFalling(Rinigin::StateContextComponent* context, MoneyBag* moneyBag);
	virtual void Enter();
	virtual Rinigin::State* Update();
	virtual void Exit();

	void Notify(Rinigin::EventArguments& eventArguments) override;
private:
	void EndSFX();
	MoneyBag* m_MoneyBag;
	bool m_Falling;
	float m_FallHeight;
};