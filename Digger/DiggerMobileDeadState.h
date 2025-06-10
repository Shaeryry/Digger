#pragma once
#include "State.h"

class DiggerMobile;
class DiggerMobileDeadState final : public Rinigin::State
{
public:
	explicit DiggerMobileDeadState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;
private:
	float m_DeadClock;
	DiggerMobile* m_DiggerMobile;
};

