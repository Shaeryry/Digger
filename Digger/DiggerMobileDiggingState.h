#pragma once
#include "State.h"
#include "Observer.h"

class DiggerMobile;
class DiggerMobileDiggingState final : public Rinigin::State, public Rinigin::Observer
{
public:
	explicit DiggerMobileDiggingState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	void Enter() override;
	void Update() override;
	void Exit() override;

	void Notify(Rinigin::EventArguments* arguments) override;
private:
	void UpdateAnimation();

	const char* m_DirectionName;
	DiggerMobile* m_DiggerMobile;
	
	float m_Speed = 100.f;
};

