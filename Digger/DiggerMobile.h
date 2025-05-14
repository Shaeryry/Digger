#pragma once
#include "Character.h"
#include "Observer.h"

namespace Rinigin { class StateContextComponent; }

class DestructibleEnvironmentComponent;
class DiggerMobileDiggingState;
class DiggerMobileDyingState;
class DiggerMobileDeadState;

class DiggerMobile final : public Character,public Rinigin::Observer
{
public:
	explicit DiggerMobile(int index,DestructibleEnvironmentComponent* map);

	DiggerMobileDiggingState* GetDiggingState() const { return m_DiggingState; }
	DiggerMobileDyingState* GetDyingState() const { return m_DyingState; }
	DiggerMobileDeadState* GetDeadState() const { return m_DeadState; }

	DestructibleEnvironmentComponent* Map() const { return m_Map; }
	void Notify(Rinigin::EventArguments* eventArguments) override;
private:
	int m_DiggerIndex;

	Rinigin::StateContextComponent* m_DiggerMobileStateContext;
	DiggerMobileDiggingState* m_DiggingState;
	DiggerMobileDyingState* m_DyingState;
	DiggerMobileDeadState* m_DeadState;

	DestructibleEnvironmentComponent* m_Map;
};

