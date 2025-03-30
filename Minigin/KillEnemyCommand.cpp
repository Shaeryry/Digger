#include "KillEnemyCommand.h"
#include "EventTypes.h"

KillEnemyCommand::KillEnemyCommand() :
	m_EnemyDiedEvent{ std::make_unique<Rinigin::Event>( Rinigin::EventArguments("EnemyDied")) }
{
}

void KillEnemyCommand::Execute()
{
	m_EnemyDiedEvent->NotifyObservers();
}
 