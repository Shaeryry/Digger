#pragma once
#include "Item.h"
#include "Observer.h"

class Emerald final : public Item,public Rinigin::Observer
{
public:
	explicit Emerald();
	Rinigin::Event* GetCollectedEvent() const { return m_CollectEvent.get(); }
	virtual void Notify(Rinigin::EventArguments & eventArguments) override;
private:
	std::unique_ptr<Rinigin::Event> m_CollectEvent;
};

