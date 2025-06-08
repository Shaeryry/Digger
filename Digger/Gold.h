#pragma once
#include "Item.h"
#include "Observer.h"

namespace Rinigin {
	class ColliderComponent;
	class SpriteSheetComponent;
	class SpriteAnimatorComponent;
}

class Gold final : public Item, public Rinigin::Observer
{
public:
	explicit Gold(Level* level);
	Rinigin::Event* GetCollectedEvent() const { return m_CollectEvent.get(); }
	Rinigin::ColliderComponent* GetTrigger() const { return m_Trigger; }
	virtual void Notify(Rinigin::EventArguments& eventArguments) override;

	virtual Item* Clone() override;
private:
	Rinigin::SpriteSheetComponent* m_SpriteSheetComponent;
	Rinigin::SpriteAnimatorComponent* m_Animator;

	Rinigin::ColliderComponent* m_Trigger;
	std::unique_ptr<Rinigin::Event> m_CollectEvent;
};

