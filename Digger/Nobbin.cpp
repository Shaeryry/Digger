#include "Nobbin.h"
#include "ColliderComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteSheetComponent.h"
#include "RigidbodyComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

#include "DiggerConstants.h"

Nobbin::Nobbin(Level* level) :
	Character("Nobbin.png"),
	m_Level(level)
{
	GetCollider()->SetBounds({ 40,40,0 });
	GetTrigger()->SetBounds({40, 40, 0});
	GetRigidbody()->SetKinematic(true);


	GetCollider()->SetLayer("Nobbin");
	GetCollider()->AddExcludedLayer("Nobbin");
	GetCollider()->AddExcludedLayer("Player");

	SetSpeed(DIGGER::NOBBIN_SPEED);

	GetHealthComponent()->SetMaxHealth(1);

	// Spritesheet
	Rinigin::SpriteSheetComponent* spriteSheet = GetSpriteSheetComponent();
	spriteSheet->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);
	spriteSheet->SetTileWidth(32);
	spriteSheet->SetTileHeight(30);
	spriteSheet->SetRows(4);
	spriteSheet->SetColumns(3);

	// Animations
	Rinigin::SpriteAnimatorComponent* animator = GetAnimator();

	animator->AddAnimation("Nobbin", Rinigin::SpriteAnimationData{ 0,0,4,DIGGER::NOBBIN_ANIMATION_LENGTH,true });
	animator->AddAnimation("HobbinRight", Rinigin::SpriteAnimationData{ 1,0,4,DIGGER::NOBBIN_ANIMATION_LENGTH,true });
	animator->AddAnimation("HobbinLeft", Rinigin::SpriteAnimationData{ 2,0,4,DIGGER::NOBBIN_ANIMATION_LENGTH,true });

	animator->PlayAnimation("Nobbin");
}

Nobbin* Nobbin::Clone()
{
	return new Nobbin(m_Level);
}

void Nobbin::Notify(Rinigin::EventArguments& arguments)
{
	switch (arguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("Died"):
		GetCharacterObject()->Destroy();
		break;
	default:
		break;
	}
}
