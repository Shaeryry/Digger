#include "Timer.h"
#include <iostream>

void dae::Timer::Update()
{
	const auto current_time = std::chrono::high_resolution_clock::now();
	const float calculated_delta_time = std::chrono::duration<float>(current_time - last_time).count(); 
	last_time = current_time;
	lag += calculated_delta_time;

	deltaTime = calculated_delta_time; // Update the delta time
}

void dae::Timer::FixedUpdate()
{
	lag -= FIXED_TIME_STEP;
}


float dae::Timer::GetFPS()
{
	return (1 / deltaTime);
}
