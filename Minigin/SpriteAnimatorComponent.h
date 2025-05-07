#pragma once
#include "Component.h"

// TODO : Consider adding events to this
namespace Rinigin {
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

		void PlayAnimation(const SpriteAnimationData& data);
		const SpriteAnimationData& GetCurrentAnimation() const { return m_CurrentAnimation; }
		// TODO : ADD ANIMATION STARTED EVENT
		// TODO : ADD ANIMATION ENDED EVENT
		// TODO : ADD ANIMATION COMPLETED EVENT(Whenever the animation completes a loop, so while this can be the same as ended it doesn't have to be)
	private:
		void ResetStartTime();
		bool m_Ended;

		SpriteSheetComponent* m_SpriteSheet;
		SpriteAnimationData m_CurrentAnimation;
		float m_CurrentAnimationClock;
	};
}

