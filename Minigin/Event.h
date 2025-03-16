#pragma once
#include <vector>
#include "EventTypes.h"

namespace dae {
	class GameObject;
	class Observer;

	class Event final
	{
	public:
		explicit Event();
		~Event() = default;
		Event(const Event& other) = delete;
		Event(Event&& other) = delete; 
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void NotifyObservers(EventType eventType,GameObject* gameObject);
	private:
		bool IsObserver(Observer* observer);
		std::vector<Observer*> m_Observers;
	};
}


