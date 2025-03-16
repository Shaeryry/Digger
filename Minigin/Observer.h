#pragma once
#include "EventTypes.h"

namespace dae {
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(dae::EventType eventType,GameObject* gameObject) = 0;
	};
}

