#ifndef PH_RELEASE

#include "collisionDebug.hpp"

using PopHead::Physics::CollisionDebugSettings;

void CollisionDebugSettings::turnOn()
{
	CollisionDebugRect::setShouldDisplay(true);
}

void CollisionDebugSettings::turnOff()
{
	CollisionDebugRect::setShouldDisplay(false);
}

#endif //!PH_RELEASE
