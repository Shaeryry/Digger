#include "EventTypes.h"
#include "GameObject.h"
#include "Helpers.h"


GameObjectEventArguments::GameObjectEventArguments(const char* eventName, Rinigin::GameObject* gameObject) :
	EventArguments(eventName),
	m_GameObject{ gameObject }
{
}
