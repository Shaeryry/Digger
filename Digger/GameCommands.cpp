#include "GameCommands.h"
#include "GameScreenState.h"
#include "ServiceLocator.h"

void GameCommands::SkipLevelCommand::Execute()
{
	m_GameState->NextLevel();
}

void GameCommands::MuteGameCommand::Execute()
{
	Rinigin::ServiceLocator::GetSoundService().SetMuted(not Rinigin::ServiceLocator::GetSoundService().IsMuted());
}
