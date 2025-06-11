#include "GameCommands.h"
#include "GameScreenState.h"

void GameCommands::SkipLevelCommand::Execute()
{
	m_GameState->NextLevel();
}
