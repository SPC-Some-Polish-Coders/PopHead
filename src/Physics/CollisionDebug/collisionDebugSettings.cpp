#ifndef PH_RELEASE

#include "collisionDebugSettings.hpp"

using PopHead::Physics::CollisionDebugSettings;

CollisionDebugSettings::CollisionDebugSettings()
{
	turnOff();
}

void CollisionDebugSettings::turnOn()
{
	CollisionDebugRect::setShouldDisplay(true);
}

void CollisionDebugSettings::turnOff()
{
	CollisionDebugRect::setShouldDisplay(false);
}

#endif //!PH_RELEASE
