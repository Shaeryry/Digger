#pragma once
#include <vector>
#include <memory>
#include "EventArguments.h"

namespace Rinigin {
	class GameObject;
	class Observer;

	class Event final
	{
	public:
		template<typename ArgumentsType> explicit Event(const ArgumentsType& eventData) : m_EventData{ std::make_unique<ArgumentsType>(eventData) } {};

		~Event() = default;
		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void NotifyObservers();
	private:
		bool IsObserver(Observer* observer);
		std::vector<Observer*> m_Observers;
		std::unique_ptr<EventArguments> m_EventData;
	};
}


