#pragma once
#include "Item.h"

namespace Rinigin {
	class GameObject;
	class ColliderComponent;
	class TextureComponent;
	class SpriteSheetComponent;
	class SpriteAnimatorComponent;
	class StateContextComponent;
}

namespace Rinigin { class RigidbodyComponent; class ColliderComponent; }
class MoneyBag final : public Item
{
public:
	explicit MoneyBag(Level* level);

	Rinigin::SpriteAnimatorComponent* GetAnimator() const { return m_Animator; };
	Rinigin::SpriteSheetComponent* GetSpriteSheetComponent() const { return m_SpriteSheetComponent; };
	Rinigin::ColliderComponent* GetCollider() const { return m_ColliderComponent; };
	Rinigin::StateContextComponent* GetStateMachine() const { return m_StateMachineComponent; }
	Level* GetLevel() const { return m_Level; }
	Rinigin::RigidbodyComponent* GetRigidbody() const { return m_RigidbodyComponent; }

	virtual Item* Clone() override;
private:
	Rinigin::ColliderComponent* m_ColliderComponent;
	Rinigin::RigidbodyComponent* m_RigidbodyComponent;

	Rinigin::SpriteSheetComponent* m_SpriteSheetComponent;
	Rinigin::SpriteAnimatorComponent* m_Animator;

	Rinigin::StateContextComponent* m_StateMachineComponent;
};

