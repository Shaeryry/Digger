#include "SpriteAnimatorComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteSheetComponent.h"
#include "Timer.h"
#include "Helpers.h"
#include <iostream>
#include "Event.h"

Rinigin::SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* gameObject, SpriteSheetComponent* spriteSheetComponent) :
	Component(gameObject),
	m_Ended(true),
	m_SpriteSheet(spriteSheetComponent),
	m_CurrentAnimation(),
	m_CurrentAnimationClock(0),

	m_StartedEvent( std::make_unique<Rinigin::Event>( Rinigin::NullEventArguments("AnimationStart") ) ),
	m_EndedEvent( std::make_unique<Rinigin::Event>( Rinigin::NullEventArguments("AnimationEnded") ) ),
	m_CompletedEvent( std::make_unique<Rinigin::Event>( Rinigin::NullEventArguments("AnimationCompleted") ) )

{
}

void Rinigin::SpriteAnimatorComponent::PlayAnimation(const char* animationName)
{
	SpriteAnimationData* data = GetAnimation(animationName);
	if (not data) {
		std::cerr << "Animation doesn't exist !" << std::endl;
		return;
	} // TODO : Animation doesn't exist yet, maybe warn them about it

	if (m_SpriteSheet) {
		m_SpriteSheet->SetCurrentRow(data->startRow);
		m_SpriteSheet->SetCurrentColumn(data->startColumn);
	}

	ResetStartTime();
	m_CurrentAnimation = data; // Set the current animation
	m_Ended = false; // Make the animation uncomplete

	m_StartedEvent->NotifyObservers();
}

void Rinigin::SpriteAnimatorComponent::AddAnimation(const char* animationName, const SpriteAnimationData& data)
{

	const unsigned int key{ Rinigin::Helpers::sdbm_hash(animationName) };
	std::unique_ptr<SpriteAnimationData> animationDataPtr{ std::make_unique<SpriteAnimationData>(data) };
	m_Animations.emplace(key, std::move(animationDataPtr));
	// TODO : Add a warning to tell that an animation is being overwritten
}

Rinigin::SpriteAnimationData* Rinigin::SpriteAnimatorComponent::GetAnimation(unsigned int id) const
{
	auto animationIt = m_Animations.find(id);
	if (animationIt != m_Animations.end()) {
		return animationIt->second.get();
	}
	else {
		// TODO : Throw exception telling you, that you fucked up animation
		std::cerr << "Animation not found !" << std::endl;
	}

	return nullptr;
}

Rinigin::SpriteAnimationData* Rinigin::SpriteAnimatorComponent::GetAnimation(const char* animationName) const
{
	return GetAnimation( Rinigin::Helpers::sdbm_hash(animationName) );
}

void Rinigin::SpriteAnimatorComponent::ResetStartTime()
{
	m_CurrentAnimationClock = 0;

}

void Rinigin::SpriteAnimatorComponent::Update()
{
	if (m_CurrentAnimation) {
		if (m_SpriteSheet) {
				const int frameOffset = m_CurrentAnimation->startRow;
				const int frames = m_CurrentAnimation->frames;
				const bool isLooping = m_CurrentAnimation->looping;

				//if (m_Ended and not isLooping) return; // Prevent calculation if you're not looping and the animation ended
				if (frames < 1) return; // If there are no frames then no need to calculate;

				const float animationElapsedTime = m_CurrentAnimationClock;
				const float timePerFrame = (m_CurrentAnimation->animationLength / frames);
				const int calculatedFrame = static_cast<int>(std::floor((animationElapsedTime / timePerFrame)));
				const int currentFrame = std::min(calculatedFrame, (frames - 1));

				if (not m_Ended) {

					if (calculatedFrame >= frames) {
						// TODO : Fire completed event
						m_CompletedEvent->NotifyObservers();

						// Looping
						if (isLooping) {
							ResetStartTime();
						}
						else { 
							// TODO : Fire ended event
							m_Ended = true; // Animation ended
							m_EndedEvent->NotifyObservers();
						}
					}

					m_SpriteSheet->SetCurrentRow(frameOffset + currentFrame);
				}

			}
	}
	
	m_CurrentAnimationClock += Timer::GetInstance().deltaTime;
}
