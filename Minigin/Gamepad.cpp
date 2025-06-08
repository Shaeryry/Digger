#include "Gamepad.h"
#include "Command.h"
#include "InputManager.h"

#include <windows.h>
#include "Xinput.h"

// Pimpl

class Rinigin::Gamepad::impl final {
	public:
		explicit impl(const int index) : m_ControllerIndex{ index } {};
		void Process();
		bool IsButtonTriggered(unsigned int button) const;
		bool IsButtonReleased(unsigned int button) const;
		bool IsDown(unsigned int button) const;
		bool IsKeyboard() const { return (m_ControllerIndex == -1); }

		Binding* AddBinding(const Binding& binding);
		bool RemoveBinding(Binding* binding);

		int GetGamepadIndex() const { return m_ControllerIndex; }
		const std::vector<std::unique_ptr<Binding>>& GetBindings() { return m_Bindings; };
	private:
		std::vector<std::unique_ptr<Binding>> m_Bindings; // List of all the bindings the controllers has

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
		int m_ControllerIndex{};
};  


void Rinigin::Gamepad::impl::Process()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Rinigin::Gamepad::impl::IsButtonTriggered(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
bool Rinigin::Gamepad::impl::IsButtonReleased(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
bool Rinigin::Gamepad::impl::IsDown(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }

Rinigin::Binding* Rinigin::Gamepad::impl::AddBinding(const Binding& binding)
{
	std::unique_ptr<Binding> newBinding{ std::make_unique<Binding>(binding) };
	m_Bindings.emplace_back(std::move(newBinding));
	return m_Bindings.back().get();
}

bool Rinigin::Gamepad::impl::RemoveBinding(Binding* binding)
{
	if (!binding) return false; // If there is no binding then simply don't perform this

    auto removedIt = std::remove_if(m_Bindings.begin(), m_Bindings.end(),
		[&](std::unique_ptr<Binding>& currentbinding)
		{
			return currentbinding.get() == binding;
		}
    );

	if (removedIt != m_Bindings.end()) {
		m_Bindings.erase(removedIt, m_Bindings.end());
		return true;
	};

    return false;
}

// Gamepad implementation

Rinigin::Gamepad::Gamepad(const int index) :
	m_XInputPimpl{ std::make_unique<impl>(index) }
{
} 

Rinigin::Gamepad::~Gamepad()
{
}

void Rinigin::Gamepad::ExecuteCommands(const ExecutionCommandInfo& commandInfo)
{
	for (auto& binding : m_XInputPimpl->GetBindings() ) {
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
			case Rinigin::BindingConnection::Down:
				canExecute = IsButtonTriggered(button);
				break;
			case Rinigin::BindingConnection::Release:
				canExecute = IsButtonReleased(button);
				break;
			case Rinigin::BindingConnection::Held:
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
			case Rinigin::BindingConnection::Down:
				canExecute = typeEvent == SDL_KEYDOWN and validPress; //IsButtonTriggered(button);
				break;
			case Rinigin::BindingConnection::Release:
				canExecute = typeEvent == SDL_KEYUP and validPress;//IsButtonReleased(button);
				break;
			case Rinigin::BindingConnection::Held:
				canExecute = not commandInfo.polling and commandInfo.keyboardState[button];
				break;
			}
		}
		

		if (canExecute) {
			binding->command->Execute();
		};
	}
}

Rinigin::Binding* Rinigin::Gamepad::AddBinding(GamepadButton button, BindingConnection connectionType, Command* command)
{
	return AddBinding(static_cast<unsigned int>(button), connectionType, command);
}
Rinigin::Binding* Rinigin::Gamepad::AddBinding(int button, BindingConnection connectionType, Command* command)
{
	return AddBinding(static_cast<unsigned int>(button), connectionType, command);
}

Rinigin::Binding* Rinigin::Gamepad::AddBinding(unsigned int button, BindingConnection connectionType, Command* command)
{
	Binding newBinding{};
	newBinding.button = button;
	newBinding.connectionType = connectionType;
	newBinding.command = command;

	return m_XInputPimpl->AddBinding(newBinding);
}

Rinigin::Binding* Rinigin::Gamepad::AddBinding(const Binding& binding)
{
	return m_XInputPimpl->AddBinding(binding);
}

void Rinigin::Gamepad::RemoveBinding(Binding* binding)
{
	m_XInputPimpl->RemoveBinding(binding); 
}

bool Rinigin::Gamepad::IsButtonTriggered(unsigned int button) const { return m_XInputPimpl->IsButtonTriggered(button);  }
bool Rinigin::Gamepad::IsButtonReleased(unsigned int button) const { return m_XInputPimpl->IsButtonReleased(button); }
bool Rinigin::Gamepad::IsDown(unsigned int button) const { return m_XInputPimpl->IsDown(button); }

bool Rinigin::Gamepad::IsButtonTriggered(int button) const { return IsButtonTriggered(static_cast<unsigned int>(button)); }
bool Rinigin::Gamepad::IsButtonReleased(int button) const { return IsButtonReleased(static_cast<unsigned int>(button)); }
bool Rinigin::Gamepad::IsDown(int button) const { return IsDown(static_cast<unsigned int>(button)); }

bool Rinigin::Gamepad::IsKeyboard() const { return m_XInputPimpl->IsKeyboard(); }
int Rinigin::Gamepad::GetGamepadIndex() const { return m_XInputPimpl->GetGamepadIndex(); };

void Rinigin::Gamepad::Process() { m_XInputPimpl->Process(); };
