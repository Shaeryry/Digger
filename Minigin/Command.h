#pragma once

namespace Rinigin {
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

