#pragma once
#include <memory>
#include "Command.h"
#include "Event.h"

class KillEnemyCommand final : public Rinigin::Command
{
public:
	explicit KillEnemyCommand();
	virtual void Execute() override;

	Rinigin::Event* GetEnemyDiedEvent() const { return m_EnemyDiedEvent.get(); };
private:
	std::unique_ptr<Rinigin::Event> m_EnemyDiedEvent; 
};

