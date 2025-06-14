#pragma once
#include <State.h>

class NobbinIdleState : public Rinigin::State
{
public:
	using Rinigin::State::State;

	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;
private:
};

