#include "MoneyBag.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"

#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"
#include "StateContextComponent.h"

#include "MoneyBagIdle.h"
#include "MoneyBagPrefall.h"
#include "MoneyBagFalling.h"

#include "Level.h"
#include "Scene.h"

MoneyBag::MoneyBag(Level* level) :
	Item(level,"MoneyBag.png"),
	m_RigidbodyComponent(nullptr),
	m_ColliderComponent(nullptr),
	m_StateMachineComponent(nullptr)
{
	m_ColliderComponent = GetItemObject()->AddComponent<Rinigin::ColliderComponent>(glm::vec3{ 30,30,0 }, glm::vec3{ 0,0,0 },false);
	m_TriggerComponent = GetItemObject()->AddComponent<Rinigin::ColliderComponent>(glm::vec3{ 30,15,0 }, glm::vec3{ 0,0,0 }, true);
	m_TriggerComponent->SetLayer("MoneyBag");

	m_RigidbodyComponent = GetItemObject()->AddComponent<Rinigin::RigidbodyComponent>(m_ColliderComponent,1.f,false);
	m_RigidbodyComponent->SetAxisLock(1, 0, 1);

	m_SpriteSheetComponent = GetItemObject()->AddComponent<Rinigin::SpriteSheetComponent>(GetRenderer(), GetTextureComponent());
	m_Animator = GetItemObject()->AddComponent<Rinigin::SpriteAnimatorComponent>(m_SpriteSheetComponent);

	// Sprite sheet
	m_SpriteSheetComponent->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);
	m_SpriteSheetComponent->SetRows(2);
	m_SpriteSheetComponent->SetColumns(3);

	// Animations
	m_Animator->AddAnimation("Idle", Rinigin::SpriteAnimationData{ 0,0,1,1,true });
	m_Animator->AddAnimation("Falling", Rinigin::SpriteAnimationData{ 1,0,1,1,true });
	m_Animator->AddAnimation("Prefall", Rinigin::SpriteAnimationData{ 2,0,2,1,true });

	// State Machine

	m_StateMachineComponent = GetItemObject()->AddComponent<Rinigin::StateContextComponent>();
	MoneyBagIdle* idle = m_StateMachineComponent->CreateState<MoneyBagIdle>(this);
	m_StateMachineComponent->CreateState<MoneyBagPrefall>(this);
	m_StateMachineComponent->CreateState<MoneyBagFalling>(this);


	m_StateMachineComponent->SetState(idle);
}

Item* MoneyBag::Clone()
{
	MoneyBag* newMoneyBag = new MoneyBag(m_Level);
	newMoneyBag->GetItemObject()->SetActive(true);
	return newMoneyBag;
}

