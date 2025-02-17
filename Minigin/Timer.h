#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae {
	class Timer : public Singleton<Timer>
	{
	public:
		std::chrono::high_resolution_clock::time_point UpdateTimer();
		void FixedUpdate();
		float GetFPS();

		float deltaTime{ 0.0f };
		float lag{ 0.0f };
		const int MAX_FPS{ 60 };
		const int MS_PER_FRAME{ 1000 / MAX_FPS };
		const float FIXED_TIME_STEP{ 1.f / MAX_FPS };
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> last_time{ std::chrono::high_resolution_clock::now() };
	};
}


