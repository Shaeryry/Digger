#include "EventTypes.h"
#include "GameObject.h"
#include "Helpers.h"


GameObjectEventArguments::GameObjectEventArguments(const char* eventName, Rinigin::GameObject* gameObject) :
	EventArguments(eventName),
	m_GameObject{ gameObject }
{
}

ScoreArguments::ScoreArguments(const char* eventName, int addedValue) :
	EventArguments(eventName),
	m_AddedValue(addedValue)
{

}
