#include "MenuCommand.h"


MenuCommand::MenuCommand(const char* menuEventName) :
	m_MenuName(menuEventName),
	m_InputEvent(std::make_unique<Rinigin::Event>() )
{
}

void MenuCommand::Execute() 
{
	Rinigin::NullEventArguments arguments{ m_MenuName };
	m_InputEvent->NotifyObservers(arguments);
}
