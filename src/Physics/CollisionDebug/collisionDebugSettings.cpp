#ifndef PH_RELEASE

#include "collisionDebugSettings.hpp"
#include "Physics/bodyType.hpp"

using PopHead::Physics::CollisionDebugSettings;
using PopHead::Physics::BodyType;

CollisionDebugSettings::CollisionDebugSettings()
	:mShouldDisplay(false)
	,mShouldDisplayKinematicBodies(true)
	,mShouldDisplayStaticBodies(true)
{
}

void CollisionDebugSettings::turnOn()
{
	mShouldDisplay = true;
}

void CollisionDebugSettings::turnOff()
{
	mShouldDisplay = false;
}

void CollisionDebugSettings::displayOnlyKinematicBodies()
{
	mShouldDisplayKinematicBodies = true;
	mShouldDisplayStaticBodies = false;
}

void CollisionDebugSettings::displayOnlyStaticBodies()
{
	mShouldDisplayKinematicBodies = false;
	mShouldDisplayStaticBodies = true;
}

void CollisionDebugSettings::displayAllBodies()
{
	mShouldDisplayKinematicBodies = true;
	mShouldDisplayStaticBodies = true;
}

bool CollisionDebugSettings::shouldDisplay(BodyType bodyType)
{
	if (bodyType == BodyType::kinematicBody && !mShouldDisplayKinematicBodies)
		return false;
	if (bodyType == BodyType::staticBody && !mShouldDisplayStaticBodies)
		return false;

	return mShouldDisplay;
}

#endif //!PH_RELEASE
