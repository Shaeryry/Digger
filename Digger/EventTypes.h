#pragma once
#include "Event.h"

namespace Rinigin { class GameObject; };

class GameObjectEventArguments final : public Rinigin::EventArguments {
public:
	explicit GameObjectEventArguments(const char* eventName, Rinigin::GameObject* gameObject);
	Rinigin::GameObject* GetGameObject() const { return m_GameObject; };
private:
	Rinigin::GameObject* m_GameObject; 
}; 