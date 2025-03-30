#pragma once
#include <memory>
#include <vector>
#include <SDL.h>

namespace Rinigin {
	class Command;

	enum class BindingConnection {
		OnTrigger,
		OnRelease,
		OnHeld
	};

	enum class GamepadButton : unsigned int {
		MINIGIN_GAMEPAD_DPAD_UP = 0x0001,
		MINIGIN_GAMEPAD_DPAD_DOWN = 0x0002,
		MINIGIN_GAMEPAD_DPAD_LEFT = 0x0004,
		MINIGIN_GAMEPAD_DPAD_RIGHT = 0x0008,
		MINIGIN_GAMEPAD_A = 0x1000,
		MINIGIN_GAMEPAD_B = 0x2000,
		MINIGIN_GAMEPAD_X = 0x4000,
		MINIGIN_GAMEPAD_Y = 0x8000,

	};

	struct ExecutionCommandInfo {
		bool polling;
		SDL_Event* event;
		const Uint8* keyboardState;
	};

	struct Binding {
		unsigned int button;
		Command* command;
		BindingConnection connectionType = BindingConnection::OnTrigger;
	};

	class Gamepad final
	{
	public:
		explicit Gamepad(const int index);
		virtual ~Gamepad();
		Gamepad(const Gamepad& other) = delete;
		Gamepad(Gamepad&& other) = delete;
		Gamepad& operator=(const Gamepad& other) = delete;
		Gamepad& operator=(Gamepad&& other) = delete;

		int GetGamepadIndex() const;

		void Process();
		void ExecuteCommands(const ExecutionCommandInfo& commandInfo); // Execute all bindings, if you can !

		void AddBinding(GamepadButton button, BindingConnection connectionType, Command* command);
		void AddBinding(unsigned int button, BindingConnection connectionType, Command* command);
		void AddBinding(int button, BindingConnection connectionType, Command* command);
		void AddBinding(const Binding& binding);

		bool IsKeyboard() const;

		// Int Buttons
		bool IsButtonTriggered(int button) const;
		bool IsButtonReleased(int button) const;
		bool IsDown(int button) const;

		// Unsigned int buttons
		bool IsButtonTriggered(unsigned int button) const;
		bool IsButtonReleased(unsigned int button) const;
		bool IsDown(unsigned int button) const;

	private:
		class impl;
		std::unique_ptr<impl> m_XInputPimpl;
		//std::vector<std::unique_ptr<Binding>> m_Bindings; // List of all the bindings the controllers has
	};
}


