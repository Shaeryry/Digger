#pragma once
#include <vector>
#include <memory>
#include "Helpers.h"

namespace Rinigin {
	class Observer;

	class EventArguments {
		public:
			explicit EventArguments(const char* eventName) : EventArguments(Rinigin::Helpers::sdbm_hash(eventName)) {};
			explicit EventArguments(const unsigned int id) : m_Id(id) {};

			virtual ~EventArguments() = default;
			unsigned int GetID() const { return m_Id; };
		private:
			unsigned int m_Id;
	};

	class NullEventArguments final : public EventArguments {
	public:
		explicit NullEventArguments(const char* eventName) : EventArguments(eventName) {};
	private:
	};

	class Event final
	{
	public:
		explicit Event() = default;
		~Event() = default;
		Event(const Event& other) = delete;
		Event(Event&& other) = delete;
		Event& operator=(const Event& other) = delete;
		Event& operator=(Event&& other) = delete;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void NotifyObservers(EventArguments& args);
	private:
		bool IsObserver(Observer* observer);
		std::vector<Observer*> m_Observers;
		//std::unique_ptr<EventArguments> m_EventData;
	};
}


