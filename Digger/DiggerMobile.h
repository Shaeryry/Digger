#pragma once
#include "Character.h"
#include "Observer.h"

namespace Rinigin { class StateContextComponent; }

class ScoreComponent;
class TerrainComponent;
class DiggerMobileDiggingState;
class DiggerMobileDyingState;
class DiggerMobileDeadState;

class DiggerMobile final : public Character,public Rinigin::Observer
{
public:
	explicit DiggerMobile(int index,TerrainComponent* map);

	DiggerMobileDiggingState* GetDiggingState() const { return m_DiggingState; }
	DiggerMobileDyingState* GetDyingState() const { return m_DyingState; }
	DiggerMobileDeadState* GetDeadState() const { return m_DeadState; }
	ScoreComponent* GetScoreComponent() const { return m_ScoreComponent; }

	TerrainComponent* Map() const { return m_Map; }
	void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	int m_DiggerIndex;

	ScoreComponent* m_ScoreComponent;
	Rinigin::StateContextComponent* m_DiggerMobileStateContext;
	DiggerMobileDiggingState* m_DiggingState;
	DiggerMobileDyingState* m_DyingState;
	DiggerMobileDeadState* m_DeadState;

	TerrainComponent* m_Map;
};

