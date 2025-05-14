#include "DiggerMobile.h"
#include "TextureComponent.h"
#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"
#include "StateContextComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "DestructibleEnviromentComponent.h"
#include "DiggerMobileDiggingState.h"
#include "DiggerMobileDyingState.h"
#include "DiggerMobileDeadState.h"
#include "Helpers.h"
#include "ColliderComponent.h"

DiggerMobile::DiggerMobile(int index,DestructibleEnvironmentComponent* map) :
	Character("DiggerMobile.png"),
	m_DiggerMobileStateContext(nullptr),
	m_DyingState(nullptr), 
	m_DeadState(nullptr),
	m_DiggingState(nullptr),
	m_DiggerIndex(index),
	m_Map(map)
{
	// Setup components
	GetCollider()->SetBounds(glm::vec3{40,40,0});
	GetHealthComponent()->GetDiedEvent()->AddObserver(this);
	GetHealthComponent()->SetMaxHealth(1);

	// Setup sprite
	Rinigin::SpriteSheetComponent* spriteSheet = GetSpriteSheetComponent();
	spriteSheet->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);
	spriteSheet->SetTileWidth(32);
	spriteSheet->SetTileHeight(30);
	spriteSheet->SetRows(12);
	spriteSheet->SetColumns(6);

	// Animations

	Rinigin::SpriteAnimatorComponent* animator = GetAnimator();

	const float t = 0.2f;
	animator->AddAnimation("Right", Rinigin::SpriteAnimationData{0,(6 * m_DiggerIndex),6,t,true} );
	animator->AddAnimation("Up", Rinigin::SpriteAnimationData{ 1,(6 * m_DiggerIndex),6,t,true });
	animator->AddAnimation("Left", Rinigin::SpriteAnimationData{ 2,(6 * m_DiggerIndex),6,t,true });
	animator->AddAnimation("Down", Rinigin::SpriteAnimationData{ 3,(6 * m_DiggerIndex),6,t,true });

	animator->AddAnimation("Dying", Rinigin::SpriteAnimationData{ 4,(1 * m_DiggerIndex),1,1,false });
	animator->AddAnimation("Dead", Rinigin::SpriteAnimationData{ 5,0,5,1.f,false });

	// States
	m_DiggerMobileStateContext = GetCharacterObject()->AddComponent<Rinigin::StateContextComponent>();
	m_DiggingState = m_DiggerMobileStateContext->CreateState<DiggerMobileDiggingState>(this);
	m_DyingState = m_DiggerMobileStateContext->CreateState<DiggerMobileDyingState>(this);
	m_DeadState = m_DiggerMobileStateContext->CreateState<DiggerMobileDeadState>(this);

	m_DiggerMobileStateContext->SetState(m_DiggingState);
}

void DiggerMobile::Notify(Rinigin::EventArguments* eventArguments)
{
	switch (eventArguments->GetID())
	{
	case Rinigin::Helpers::sdbm_hash("Died"): 
		m_DiggerMobileStateContext->SetState(m_DyingState);
		break;
	default:
		break;
	}
}
 