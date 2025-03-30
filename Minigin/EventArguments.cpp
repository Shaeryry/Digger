#include "Helpers.h"
#include "EventArguments.h"


Rinigin::EventArguments::EventArguments(const char* eventName)
	: EventArguments(Rinigin::Helpers::sdbm_hash(eventName))
{
}

Rinigin::EventArguments::EventArguments(const unsigned int id) : m_Id(id)
{
}