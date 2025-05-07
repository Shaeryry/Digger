#pragma once
#include "Component.h"
#include <map>
#include <memory>

// TODO : Consider adding events to this
// TODO : Consider adding unique ptr for animations?

namespace Rinigin {
	class Event;
	class GameObject;
	class SpriteSheetComponent;

	struct SpriteAnimationData {
		int startColumn = 0; // Corresponds to the start column
		int startRow = 0; // Corresponds to the column
		int frames = 0; // Amount of frames

		float animationLength = 1.f; // How long the animation should take to complete
		bool looping = false;
	};

	class SpriteAnimatorComponent : public Component
	{
	public:
		explicit SpriteAnimatorComponent(GameObject* gameObject,SpriteSheetComponent* spriteSheetComponent);
		virtual void Update() override;

		void PlayAnimation(const char* animationName);
		void AddAnimation(const char* animationName, const SpriteAnimationData& data);

		SpriteAnimationData* GetAnimation(unsigned int id) const;
		SpriteAnimationData* GetAnimation(const char* animationName) const;
		SpriteAnimationData* GetCurrentAnimation() const { return m_CurrentAnimation; };
		 
		Rinigin::Event* GetStartedEvent() const { return m_StartedEvent.get(); }
		Rinigin::Event* GetEndedEvent() const { return m_EndedEvent.get(); }
		Rinigin::Event* GetCompletedEvent() const { return m_CompletedEvent.get(); }
	private:
		void ResetStartTime();

		std::unique_ptr<Rinigin::Event> m_StartedEvent;
		std::unique_ptr<Rinigin::Event> m_EndedEvent;
		std::unique_ptr<Rinigin::Event> m_CompletedEvent;


		SpriteSheetComponent* m_SpriteSheet;
		SpriteAnimationData* m_CurrentAnimation;
		float m_CurrentAnimationClock;
		bool m_Ended; 

		std::map<unsigned int, std::unique_ptr<SpriteAnimationData>> m_Animations;
	};
}

