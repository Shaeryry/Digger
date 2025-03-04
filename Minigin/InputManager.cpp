#include <SDL.h>
#include <imgui.h>
#include "InputManager.h"
#include "imgui_impl_sdl2.h"

void dae::InputManager::Initialize(int amountGamepads)
{
	AddGamepad(-1); // Keyboard gamepad
	for (int gamepadIndex{ 0 }; gamepadIndex < amountGamepads; gamepadIndex++) {
		AddGamepad(gamepadIndex);
	}
}

bool dae::InputManager::ProcessInput()
{
	for (auto& gamepad : m_Gamepads) {
		gamepad->Process();
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...

		// Process ImGui event
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::AddGamepad(const int index)
{
	std::unique_ptr<Gamepad> gamepad{ std::make_unique<Gamepad>(index) };
	m_Gamepads.emplace_back(std::move(gamepad)); // Add the gamepad to the controllers gamepads available
}
