#pragma once
#include "Item.h"
#include "Observer.h"
#include "Prototype.h"

namespace Rinigin{
	class ColliderComponent;
}

class Emerald final : public Item,public Prototype<Emerald>,public Rinigin::Observer
{
public:
	explicit Emerald(Level* level);
	Rinigin::Event* GetCollectedEvent() const { return m_CollectEvent.get(); }
	Rinigin::ColliderComponent* GetTrigger() const { return m_Trigger; }
	virtual void Notify(Rinigin::EventArguments & eventArguments) override;

	virtual Emerald* Clone() override;
private:
	Rinigin::ColliderComponent* m_Trigger;
	std::unique_ptr<Rinigin::Event> m_CollectEvent;
};

