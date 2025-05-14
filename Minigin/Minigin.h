#pragma once
#include <string>
#include <functional>

struct SDL_Window;

namespace Rinigin
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath, int screenWidth, int screenHeight);
		~Minigin();
		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		void Run(const std::function<void()>& load);
		void SetWindowTitle(const char* windowName);
	private:
		void PrintSDLVersion();
		SDL_Window* m_Window;
	};
}