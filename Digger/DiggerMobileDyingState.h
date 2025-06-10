#pragma once
#include "State.h"
#include "Observer.h"

class DiggerMobile;
class DiggerMobileDyingState final : public Rinigin::State
{
public: 
	explicit DiggerMobileDyingState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;
private:
	DiggerMobile* m_DiggerMobile;
};

