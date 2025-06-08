#pragma once
#include "Event.h"

namespace Rinigin {
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Rinigin::EventArguments & eventArguments) = 0;
		template<typename ArgumentType> ArgumentType& GetArgumentsOfType(EventArguments& arguments) { return dynamic_cast<ArgumentType&>(arguments); };
	};

}
