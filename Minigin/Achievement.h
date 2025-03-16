#pragma once
#include "Singleton.h"
#include "Observer.h"

namespace dae {
	class GameObject;
	class Achievement final : public Singleton<Achievement>,public Observer
	{
	public:
		virtual void Notify(EventType event,GameObject* gameObject) override;
	};
}


