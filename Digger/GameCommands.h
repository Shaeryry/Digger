#pragma once
#include <Command.h>

class GameScreenState;
namespace GameCommands {
	class SkipLevelCommand : public Rinigin::Command
	{
	public:
		explicit SkipLevelCommand(GameScreenState* gameState) : m_GameState(gameState) {};
		virtual void Execute() override;
	private:
		GameScreenState* m_GameState;
	};

	class MuteGameCommand : public Rinigin::Command
	{
	public:
		explicit MuteGameCommand() = default;
		virtual void Execute() override;
	};
}


