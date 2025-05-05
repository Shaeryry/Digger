#pragma once
#include <string>
#include "Command.h"
#include "Event.h"

class MenuCommand final : public Rinigin::Command
{
public:
	explicit MenuCommand(const char* menuEventName);
	virtual void Execute() override;

	Rinigin::Event* GetInputEvent() const { return m_InputEvent.get(); };
private:
	std::unique_ptr<Rinigin::Event> m_InputEvent;
};