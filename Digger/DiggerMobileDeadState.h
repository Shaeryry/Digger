#pragma once
#include "State.h"

class DiggerMobile;
class DiggerMobileDeadState final : public Rinigin::State
{
public:
	explicit DiggerMobileDeadState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	void Enter() override;
	void Update() override;
	void Exit() override;
private:
	DiggerMobile* m_DiggerMobile;
};

