#include "KillEnemyCommand.h"

dae::KillEnemyCommand::KillEnemyCommand() :
	m_EnemyDiedEvent{ std::make_unique<Event>() }
{
}

void dae::KillEnemyCommand::Execute()
{
	m_EnemyDiedEvent->NotifyObservers(EventType::EnemyDied, nullptr);
}
 