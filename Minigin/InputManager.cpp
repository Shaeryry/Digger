#include <SDL.h>
#include <imgui.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"

void Rinigin::InputManager::Initialize(int amountGamepads)
{
	m_Keyboard = AddGamepad(-1); // Keyboard gamepad
	for (int gamepadIndex{ 0 }; gamepadIndex < amountGamepads; gamepadIndex++) {
		AddGamepad(gamepadIndex);
	}
}

bool Rinigin::InputManager::ProcessInput() 
{
	SDL_Event e{};
	const Uint8* state = SDL_GetKeyboardState(NULL);

	Rinigin::ExecutionCommandInfo commandInformation{};
	commandInformation.polling = true;
	commandInformation.event = &e;
	commandInformation.keyboardState = state;

	for (auto& gamepad : m_Gamepads) {
		if (!gamepad->IsKeyboard()) {
			gamepad->Process();
			gamepad->ExecuteCommands(commandInformation);
		}
	}

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) { 
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}

		m_Keyboard->ExecuteCommands(commandInformation); // Execute keyboard commands with polling
		// Process ImGui event
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	// Execute keyboard commands without polling
	commandInformation.polling = false;
	m_Keyboard->ExecuteCommands(commandInformation);

	return true;
}

Rinigin::Gamepad* Rinigin::InputManager::AddGamepad(const int index)
{
	std::unique_ptr<Gamepad> gamepad{ std::make_unique<Gamepad>(index) };
	m_Gamepads.emplace_back(std::move(gamepad)); // Add the gamepad to the controllers gamepads available
	return m_Gamepads.back().get();
}
