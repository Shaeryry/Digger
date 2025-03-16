#include "Event.h"
#include "Observer.h"

dae::Event::Event()
{
}

void dae::Event::AddObserver(Observer* observer)
{
	if (!IsObserver(observer)) {
		m_Observers.emplace_back(observer);
	}
}

void dae::Event::RemoveObserver(Observer* observer)
{
	if (IsObserver(observer)) {
		std::erase(m_Observers, observer);
	}
}

void dae::Event::NotifyObservers(EventType eventType, GameObject* gameObject)
{
	for (auto& observer : m_Observers) {
		observer->Notify(eventType, gameObject);
	}
}

bool dae::Event::IsObserver(Observer* observer)
{
	return std::find(m_Observers.begin(), m_Observers.end(), observer) != m_Observers.end();
}
