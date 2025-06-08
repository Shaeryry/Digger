#include "MoneyBag.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"

#include "SpriteSheetComponent.h"
#include "SpriteAnimatorComponent.h"

MoneyBag::MoneyBag() : 
	Item("MoneyBag.png"),
	m_RigidbodyComponent(nullptr),
	m_ColliderComponent(nullptr)
{
	m_ColliderComponent = GetItemObject()->AddComponent<Rinigin::ColliderComponent>(glm::vec3{ 30,30,0 }, glm::vec3{ 0,0,0 },false);
	m_RigidbodyComponent = GetItemObject()->AddComponent<Rinigin::RigidbodyComponent>(m_ColliderComponent,1.f,false);

	m_SpriteSheetComponent = GetItemObject()->AddComponent<Rinigin::SpriteSheetComponent>(GetRenderer(), GetTextureComponent());
	m_Animator = GetItemObject()->AddComponent<Rinigin::SpriteAnimatorComponent>(m_SpriteSheetComponent);

	m_SpriteSheetComponent->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);
	m_SpriteSheetComponent->SetRows(1);
	m_SpriteSheetComponent->SetColumns(2);
}
