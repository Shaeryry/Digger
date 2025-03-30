#pragma once
#include "EventArguments.h"

namespace Rinigin {
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(EventArguments* arguments) = 0;
		template<typename ArgumentType> ArgumentType* GetArgumentsOfType(EventArguments* arguments) { return dynamic_cast<ArgumentType*>(arguments); };
	};

}
