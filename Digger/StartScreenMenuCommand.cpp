#include "StartScreenMenuCommand.h"


MenuCommand::MenuCommand(const char* menuEventName) :
	m_InputEvent(std::make_unique<Rinigin::Event>( Rinigin::NullEventArguments(menuEventName)) )
{
}

void MenuCommand::Execute() 
{
	m_InputEvent->NotifyObservers();
}
