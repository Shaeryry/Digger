#pragma once

namespace Rinigin {
	class ICollisionMask
	{
	public:
		ICollisionMask();
		virtual ~ICollisionMask() = default;
		virtual bool IsSolidAt(int x, int y) const = 0;
	private:
	};

}

