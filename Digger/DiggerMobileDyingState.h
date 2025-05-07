#pragma once
#include "State.h"
#include "Observer.h"

class DiggerMobile;
class DiggerMobileDyingState final : public Rinigin::State,public Rinigin::Observer
{
public: 
	explicit DiggerMobileDyingState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	void Enter() override;
	void Update() override;
	void Exit() override;

	void Notify(Rinigin::EventArguments* arguments) override;
private:
	DiggerMobile* m_DiggerMobile;
};

