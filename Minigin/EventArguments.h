#pragma once

namespace Rinigin {
	class EventArguments {
		public:
			explicit EventArguments(const char* eventName);
			explicit EventArguments(const unsigned int id); 

			virtual ~EventArguments() = default;
			unsigned int GetID() const { return m_Id; };
		private: 
			unsigned int m_Id;
	};
}
 