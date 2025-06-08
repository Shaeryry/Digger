#pragma once
#include "Item.h"

namespace Rinigin {
	class GameObject;
	class ColliderComponent;
	class TextureComponent;
	class SpriteSheetComponent;
	class SpriteAnimatorComponent;
}
namespace Rinigin { class RigidbodyComponent; class ColliderComponent; }
class MoneyBag final : public Item
{
public:
	explicit MoneyBag();

	Rinigin::SpriteAnimatorComponent* GetAnimator() const { return m_Animator; };
	Rinigin::SpriteSheetComponent* GetSpriteSheetComponent() const { return m_SpriteSheetComponent; };
private:
	Rinigin::ColliderComponent* m_ColliderComponent;
	Rinigin::RigidbodyComponent* m_RigidbodyComponent;

	Rinigin::SpriteSheetComponent* m_SpriteSheetComponent;
	Rinigin::SpriteAnimatorComponent* m_Animator;
};

