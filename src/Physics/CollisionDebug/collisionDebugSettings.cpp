#ifndef PH_RELEASE

#include "collisionDebugSettings.hpp"

using PopHead::Physics::CollisionDebugSettings;

CollisionDebugSettings::CollisionDebugSettings()
{
	turnOff();
	displayAllBodies();
}

void CollisionDebugSettings::turnOn()
{
	CollisionDebugRect::setShouldDisplay(true);
}

void CollisionDebugSettings::turnOff()
{
	CollisionDebugRect::setShouldDisplay(false);
}

void CollisionDebugSettings::displayOnlyKinematicBodies()
{
	CollisionDebugRect::setShouldDisplayKinematicBodies(true);
	CollisionDebugRect::setShouldDisplayStaticBodies(false);
}

void CollisionDebugSettings::displayOnlyStaticBodies()
{
	CollisionDebugRect::setShouldDisplayKinematicBodies(false);
	CollisionDebugRect::setShouldDisplayStaticBodies(true);
}

void CollisionDebugSettings::displayAllBodies()
{
	CollisionDebugRect::setShouldDisplayKinematicBodies(true);
	CollisionDebugRect::setShouldDisplayStaticBodies(true);
}

#endif //!PH_RELEASE
