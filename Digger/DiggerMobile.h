#pragma once
#include "Character.h"
#include "Observer.h"

namespace Rinigin { class StateContextComponent; }

class Level;
class LivesComponent;
class ScoreComponent;
class DiggerMobileDiggingState;
class DiggerMobileDyingState;
class DiggerMobileDeadState;

class DiggerMobile final : public Character,public Rinigin::Observer
{
public:
	explicit DiggerMobile(int index, Level* level);

	DiggerMobileDiggingState* GetDiggingState() const { return m_DiggingState; }
	DiggerMobileDyingState* GetDyingState() const { return m_DyingState; }
	DiggerMobileDeadState* GetDeadState() const { return m_DeadState; }
	ScoreComponent* GetScoreComponent() const { return m_ScoreComponent; }
	int GetDiggerIndex() const { return m_DiggerIndex; }

	Level* GetLevel() const { return m_Level; }
	void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	int m_DiggerIndex;

	ScoreComponent* m_ScoreComponent;

	Rinigin::StateContextComponent* m_DiggerMobileStateContext;
	DiggerMobileDiggingState* m_DiggingState;
	DiggerMobileDyingState* m_DyingState;
	DiggerMobileDeadState* m_DeadState;

	Level* m_Level;
};

