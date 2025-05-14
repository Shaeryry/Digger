#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <chrono>
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Minigin.h"
#include "ServiceLocator.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "PhysicsManager.h"
#include "Timer.h"

Rinigin::Minigin::Minigin(const std::string &dataPath,int screenWidth,int screenHeight) :
	m_Window(nullptr)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Minigin Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_OPENGL
	); 

	
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	ResourceManager::GetInstance().Init(dataPath);
	InputManager::GetInstance().Initialize(4);
}

Rinigin::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Rinigin::Minigin::Run(const std::function<void()>& load)
{
	load(); 

	auto& physics = Physics::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();

	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{ 
		auto currentTime = timer.UpdateTimer(); // Update delta time and return the current time for this update !
		doContinue = input.ProcessInput(); 

		// Fixed Update
		while (timer.lag >= timer.FIXED_TIME_STEP)
		{
			timer.FixedUpdate();
			physics.FixedUpdate();
			sceneManager.FixedUpdate();
		}

		// Regular update
		sceneManager.Update();

		// Late update
		sceneManager.LateUpdate();

		// Draw
		renderer.Render(); 
		physics.Render();

		// Sleep 
		const auto sleep_time = currentTime + std::chrono::milliseconds( timer.MS_PER_FRAME ) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}
}

void Rinigin::Minigin::SetWindowTitle(const char* windowName)
{
	SDL_SetWindowTitle(m_Window, windowName);
}


void Rinigin::Minigin::PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}