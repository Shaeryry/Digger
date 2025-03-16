#pragma once
#include <memory>
#include "Command.h"
#include "Event.h"

namespace dae { 
	class KillEnemyCommand final : public Command
	{
	public:
		explicit KillEnemyCommand();
		virtual void Execute() override;

		Event* GetEnemyDiedEvent() const { return m_EnemyDiedEvent.get(); };
	private:
		std::unique_ptr<Event> m_EnemyDiedEvent; 
	};
}

