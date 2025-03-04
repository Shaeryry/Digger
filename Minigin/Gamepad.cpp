#include "Gamepad.h"
#include "Command.h"
#include "InputManager.h"

dae::Gamepad::Gamepad(const int index) :
	m_ControllerIndex{ index },
	m_CurrentState{},
	m_PreviousState{},
	m_ButtonsPressedThisFrame{},
	m_ButtonsReleasedThisFrame{}
{
}

dae::Gamepad::~Gamepad()
{
}

void dae::Gamepad::Process()
{
	//XInput magic
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	ExecuteCommands();
}

void dae::Gamepad::ExecuteCommands()
{
	for (auto& binding : m_Bindings) {
		bool canExecute{ false };
		const unsigned int& button{ binding->button };

		switch (binding->connectionType)
		{
			case dae::BindingConnection::OnTrigger:
				canExecute = IsButtonTriggered(button);
				break;
			case dae::BindingConnection::OnRelease:
				canExecute = IsButtonReleased(button);
				break;
			case dae::BindingConnection::OnHeld:
				canExecute = IsDown(button);
				break;
		}

		if (canExecute) binding->command->Execute();
	}
}

void dae::Gamepad::AddBinding(unsigned int button, BindingConnection connectionType, Command* command)
{
	std::unique_ptr<Binding> newBinding{ std::make_unique<Binding>() };
	newBinding->button = button;
	newBinding->connectionType = connectionType;
	newBinding->command = command;

	m_Bindings.emplace_back(std::move(newBinding));
}

bool dae::Gamepad::IsButtonTriggered(unsigned int button) { return m_ButtonsPressedThisFrame & button;  }
bool dae::Gamepad::IsButtonReleased(unsigned int button) { return m_ButtonsReleasedThisFrame & button; }
bool dae::Gamepad::IsDown(unsigned int button) { return m_CurrentState.Gamepad.wButtons & button; }

