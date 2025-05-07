#include "Digger.h"
#include "TextureComponent.h"
#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"

Digger::Digger()
{
	SetSpeed(100.f);
	GetTextureComponent()->SetTexture("testSprite.jpg");
	// Setup sprite
	Rinigin::SpriteSheetComponent* spriteSheet = GetSpriteSheetComponent();
	spriteSheet->SetRows(9);
	spriteSheet->SetColumns(4);
	spriteSheet->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);

	// Animations
	Rinigin::SpriteAnimatorComponent* animator = GetAnimator();
	Rinigin::SpriteAnimationData testAnimation;
	testAnimation.frames = 9;
	testAnimation.looping = true;

	animator->PlayAnimation(testAnimation);
}
 