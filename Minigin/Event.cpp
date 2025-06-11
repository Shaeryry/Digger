#include "Event.h"
#include "Observer.h"

void Rinigin::Event::AddObserver(Observer* observer)
{
	if (!IsObserver(observer)) {
		observer->TrackEvent(this);
		m_Observers.emplace_back(observer);
	}
}

void Rinigin::Event::RemoveObserver(Observer* observer)
{
	if (IsObserver(observer)) {
		observer->UntrackEvent(this);
		std::erase(m_Observers, observer);
	}
}

void Rinigin::Event::NotifyObservers(EventArguments& args)
{
	for (auto* observer : m_Observers) {
		observer->Notify(args);
	}
}

bool Rinigin::Event::IsObserver(Observer* observer)
{
	return std::find(m_Observers.begin(), m_Observers.end(), observer) != m_Observers.end();
}
