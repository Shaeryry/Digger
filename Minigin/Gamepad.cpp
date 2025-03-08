#include "Gamepad.h"
#include "Command.h"
#include "InputManager.h"

#include "Windows.h"
#include "Xinput.h"

// Pimpl

class dae::Gamepad::impl {
	public:
		explicit impl(const int index) : m_ControllerIndex{ index } {};
		void Process();
		bool IsButtonTriggered(unsigned int button) const;
		bool IsButtonReleased(unsigned int button) const;
		bool IsDown(unsigned int button) const;
		int GetGamepadIndex() const { return m_ControllerIndex; }
		bool IsKeyboard() const { return (m_ControllerIndex == -1); }
	private:
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
		int m_ControllerIndex{};
};  


void dae::Gamepad::impl::Process()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}
bool dae::Gamepad::impl::IsButtonTriggered(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
bool dae::Gamepad::impl::IsButtonReleased(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
bool dae::Gamepad::impl::IsDown(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }

// Gamepad implementation

dae::Gamepad::Gamepad(const int index) :
	m_XInputPimpl{ std::make_unique<impl>(index) }
{
} 

dae::Gamepad::~Gamepad()
{
}

void dae::Gamepad::ExecuteCommands(const ExecutionCommandInfo& commandInfo)
{
	for (auto& binding : m_Bindings) {
		SDL_Event* event{ commandInfo.event };

		bool canExecute{ false };
		const bool useGamepadLogic{ 
			not IsKeyboard() 
			or event == nullptr
			or commandInfo.keyboardState == nullptr 
		};

		const unsigned int& button{ binding->button };
		 
		if (useGamepadLogic) {
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
		} else {
			const SDL_KeyboardEvent& key = commandInfo.event->key;
			const Uint32& typeEvent = event->type;

			const bool pressedOnce = (key.repeat < 1);
			const bool isScancode = key.keysym.scancode == static_cast<int>(button);
			const bool validPress{
				isScancode
				and pressedOnce
				and commandInfo.polling
			};

			switch (binding->connectionType)
			{
			case dae::BindingConnection::OnTrigger:
				canExecute = typeEvent == SDL_KEYDOWN and validPress; //IsButtonTriggered(button);
				break;
			case dae::BindingConnection::OnRelease:
				canExecute = typeEvent == SDL_KEYUP and validPress;//IsButtonReleased(button);
				break;
			case dae::BindingConnection::OnHeld:
				canExecute = not commandInfo.polling and commandInfo.keyboardState[button];
				break;
			}
		}
		

		if (canExecute) {
			binding->command->Execute();
		};
	}
}

void dae::Gamepad::AddBinding(GamepadButton button, BindingConnection connectionType, Command* command)
{
	AddBinding(static_cast<unsigned int>(button), connectionType, command);
}
void dae::Gamepad::AddBinding(int button, BindingConnection connectionType, Command* command)
{
	AddBinding(static_cast<unsigned int>(button), connectionType, command);
}
void dae::Gamepad::AddBinding(unsigned int button, BindingConnection connectionType, Command* command)
{
	Binding newBinding{};
	newBinding.button = button;
	newBinding.connectionType = connectionType;
	newBinding.command = command;

	AddBinding(newBinding);
}
void dae::Gamepad::AddBinding(const Binding& binding)
{
	std::unique_ptr<Binding> newBinding{ std::make_unique<Binding>(binding) };
	m_Bindings.emplace_back(std::move(newBinding));
}

bool dae::Gamepad::IsButtonTriggered(unsigned int button) const { return m_XInputPimpl->IsButtonTriggered(button);  }
bool dae::Gamepad::IsButtonReleased(unsigned int button) const { return m_XInputPimpl->IsButtonReleased(button); }
bool dae::Gamepad::IsDown(unsigned int button) const { return m_XInputPimpl->IsDown(button); }

bool dae::Gamepad::IsButtonTriggered(int button) const { return IsButtonTriggered(static_cast<unsigned int>(button)); }
bool dae::Gamepad::IsButtonReleased(int button) const { return IsButtonReleased(static_cast<unsigned int>(button)); }
bool dae::Gamepad::IsDown(int button) const { return IsDown(static_cast<unsigned int>(button)); }

bool dae::Gamepad::IsKeyboard() const { return m_XInputPimpl->IsKeyboard(); }
int dae::Gamepad::GetGamepadIndex() const { return m_XInputPimpl->GetGamepadIndex(); };

void dae::Gamepad::Process() { m_XInputPimpl->Process(); };
