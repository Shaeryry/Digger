#include "SpriteAnimatorComponent.h"
#include "SpriteSheetComponent.h"
#include "Timer.h"
#include <iostream>

Rinigin::SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* gameObject, SpriteSheetComponent* spriteSheetComponent) :
	Component(gameObject),
	m_Ended(true),
	m_SpriteSheet(spriteSheetComponent),
	m_CurrentAnimation(),
	m_CurrentAnimationClock(0)
{
}

void Rinigin::SpriteAnimatorComponent::PlayAnimation(const SpriteAnimationData& data)
{
	if (m_SpriteSheet) {
		m_SpriteSheet->SetCurrentRow(data.startRow);
		m_SpriteSheet->SetCurrentColumn(data.startColumn);
	}

	m_Ended = false; // Make the animation uncomplete
	m_CurrentAnimation = data; // Set the current animation
	ResetStartTime();
}

void Rinigin::SpriteAnimatorComponent::ResetStartTime()
{
	m_CurrentAnimationClock = 0;

}

void Rinigin::SpriteAnimatorComponent::Update()
{
	if (m_SpriteSheet) {
		const int frames = (m_CurrentAnimation.frames - 1);
		const bool isLooping = m_CurrentAnimation.looping;

		//if (m_Ended and not isLooping) return; // Prevent calculation if you're not looping and the animation ended
		if (frames < 1) return; // If there are no frames then no need to calculate;

		const float animationElapsedTime = m_CurrentAnimationClock;
		const float timePerFrame = (m_CurrentAnimation.animationLength / frames);
		const int currentFrame = std::min( static_cast<int>( std::floor( (animationElapsedTime / timePerFrame) ) ) , frames );
		
		if (not m_Ended) {
			if (currentFrame >= frames) {
				// TODO : Fire completed event

				// Looping
				if (isLooping) {
					ResetStartTime();
				}
				else {
					// TODO : Fire ended event
					std::cout << "ended animation !";
					m_Ended = true; // Animation ended
				}
			}

			m_SpriteSheet->SetCurrentRow(currentFrame);
		}
	}


	m_CurrentAnimationClock += Timer::GetInstance().deltaTime;
}
