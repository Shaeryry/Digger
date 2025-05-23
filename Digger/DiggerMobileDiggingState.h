#pragma once
#include "State.h"
#include "Observer.h"

class DiggerMobile;
class DiggerMobileDiggingState final : public Rinigin::State, public Rinigin::Observer
{
public:
	explicit DiggerMobileDiggingState(Rinigin::StateContextComponent* context, DiggerMobile* digger);
	virtual void Enter() override;
	virtual Rinigin::State* Update() override;
	virtual void Exit() override;

	void Notify(Rinigin::EventArguments* arguments) override;
private:
	void UpdateAnimation();

	const char* m_DirectionName;
	DiggerMobile* m_DiggerMobile;
	
	float m_Speed = 50.f;
	int m_TunnelSize = 20;
};

