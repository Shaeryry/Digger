#include "KillEnemyCommand.h"
#include "EventTypes.h"

KillEnemyCommand::KillEnemyCommand() :
	m_EnemyDiedEvent{ std::make_unique<Rinigin::Event>() }
{
}

void KillEnemyCommand::Execute()
{
	Rinigin::NullEventArguments arguments{"EnemyDied"};
	m_EnemyDiedEvent->NotifyObservers(arguments);
}
 