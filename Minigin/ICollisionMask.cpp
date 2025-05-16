#include "ICollisionMask.h"
#include "PhysicsManager.h"

Rinigin::ICollisionMask::ICollisionMask()
{
	Rinigin::Physics::GetInstance().AddCollisionMask(this);
}
