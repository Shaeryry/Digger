#include "Timer.h"
#include <iostream>

std::chrono::high_resolution_clock::time_point Rinigin::Timer::UpdateTimer()
{
	const auto current_time = std::chrono::high_resolution_clock::now();
	const float calculated_delta_time = std::chrono::duration<float>(current_time - last_time).count(); 
	last_time = current_time;
	lag += calculated_delta_time;

	deltaTime = calculated_delta_time; // Update the delta time
	totalTimeElapsed += calculated_delta_time; // Add accumulated time 

	return current_time;
}

void Rinigin::Timer::FixedUpdate()
{
	lag -= FIXED_TIME_STEP;
}


float Rinigin::Timer::GetFPS()
{
	return (1.f / deltaTime); 
}

float Rinigin::Timer::GetTotalElapsedSec()
{
	return totalTimeElapsed;
}
