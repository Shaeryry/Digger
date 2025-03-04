#pragma once
#include <memory>
#include <vector>

#include "Windows.h"
#include "Xinput.h"

namespace dae {
	class Command;

	enum class BindingConnection {
		OnTrigger,
		OnRelease,
		OnHeld
	};

	struct Binding {
		unsigned int button;
		Command* command;
		BindingConnection connectionType = BindingConnection::OnTrigger;
	};

	class Gamepad
	{
	public:
		explicit Gamepad(const int index);
		virtual ~Gamepad();
		Gamepad(const Gamepad& other) = delete;
		Gamepad(Gamepad&& other) = delete;
		Gamepad& operator=(const Gamepad& other) = delete;
		Gamepad& operator=(Gamepad&& other) = delete;

		int GetGamepadIndex() const { return m_ControllerIndex; }

		void Process();
		void ExecuteCommands();

		bool IsButtonTriggered(unsigned int button);
		bool IsButtonReleased(unsigned int button);
		bool IsDown(unsigned int button);

		void AddBinding(unsigned int button, BindingConnection connectionType, Command* command);
		template<typename Command,typename... CommandArguments> void AddBinding(unsigned int button, BindingConnection connectionType, CommandArguments&&... args);
	private:

		XINPUT_STATE m_CurrentState;
		XINPUT_STATE m_PreviousState;
		WORD m_ButtonsPressedThisFrame;
		WORD m_ButtonsReleasedThisFrame;

		int m_ControllerIndex;
		std::vector<std::unique_ptr<Binding>> m_Bindings; // List of all the bindings the controllers has
	};

	template<typename Command, typename ...CommandArguments>
	inline void Gamepad::AddBinding(unsigned int button, BindingConnection connectionType, CommandArguments && ...args)
	{
		//auto& input = InputManager::GetInstance();
		//AddBinding(button, input.AddCommand<Command>(args));
	}
}


