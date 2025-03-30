#pragma once

namespace Rinigin {
	class EventArguments {
		public:
			explicit EventArguments(const char* eventName);
			explicit EventArguments(const unsigned int id); 

			virtual ~EventArguments() = default;
			/*EventArguments(const EventArguments& other) = delete;
			EventArguments(EventArguments&& other) = delete;
			EventArguments& operator=(const EventArguments& other) = delete;
			EventArguments& operator=(EventArguments&& other) = delete;*/

			unsigned int GetID() const { return m_Id; };
		private: 
			unsigned int m_Id;
	};
}
