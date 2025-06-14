#include "Nobbin.h"
#include "ColliderComponent.h"
#include "SpriteAnimatorComponent.h"
#include "SpriteSheetComponent.h"
#include "RigidbodyComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"

#include "StateContextComponent.h"
#include "DiggerConstants.h"
#include "NobbinPursueState.h"
#include "NobbinIdleState.h"

#include "ScoreComponent.h"
#include "Level.h"

Nobbin::Nobbin(Level* level) :
	Character("Nobbin.png"),
	m_Level(level),
	m_EnemyDied(std::make_unique<Rinigin::Event>())
{
	GetCollider()->SetBounds({ 40,40,0 });
	GetTrigger()->SetBounds({40, 40, 0});
	GetRigidbody()->SetKinematic(true);

	GetCollider()->SetLayer("Nobbin");
	GetTrigger()->SetLayer("Nobbin");
	GetTrigger()->ColliderEnterEvent()->AddObserver(this);

	GetCollider()->AddExcludedLayer("Nobbin");
	GetCollider()->AddExcludedLayer("Player");

	SetSpeed(DIGGER::NOBBIN_SPEED);

	GetHealthComponent()->GetDiedEvent()->AddObserver(this);
	GetHealthComponent()->SetMaxHealth(1);

	// States
	m_NobbinStateContext = GetCharacterObject()->AddComponent<Rinigin::StateContextComponent>();
	m_PursueState = m_NobbinStateContext->CreateState<NobbinPursueState>(this);
	m_IdleState = m_NobbinStateContext->CreateState<NobbinIdleState>();

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
	Nobbin* newNobbin = new Nobbin(m_Level);

	Character* character = m_Level->GetPlayer(0);
	ScoreComponent* scoreComponent = character->GetCharacterObject()->GetComponent<ScoreComponent>();
	if (scoreComponent) {
		newNobbin->GetEnemyDiedEvent()->AddObserver(scoreComponent);
	}

	return newNobbin;
}

void Nobbin::Notify(Rinigin::EventArguments& arguments)
{
	switch (arguments.GetID())
	{
	case Rinigin::Helpers::sdbm_hash("CollisionEnter"): {
		Rinigin::CollisionEventArguments& collisionArgument{ GetArgumentsOfType<Rinigin::CollisionEventArguments>(arguments) };
		const unsigned int layerId{ collisionArgument.GetOther()->GetCollisionLayer() };
		const unsigned int playerLayerId{ Rinigin::Helpers::sdbm_hash("Player") };

		if (layerId == playerLayerId) {
			HealthComponent* healthComponent = collisionArgument.GetOther()->GetOwner()->GetComponent<HealthComponent>();
			if (healthComponent) {
				healthComponent->TakeDamage(1);
				RemoveEnemy();
			}
		};

		break;
	}
	case Rinigin::Helpers::sdbm_hash("Died"): {
		Rinigin::NullEventArguments eventArgs("NobbinDied");
		m_EnemyDied->NotifyObservers(eventArgs);
		RemoveEnemy();
		break;
	}
	default:
		break;
	}
}

void Nobbin::RemoveEnemy()
{
	GetCharacterObject()->Destroy();
	m_Level->GetEnemySpawner().RemoveTracked(this);
}
