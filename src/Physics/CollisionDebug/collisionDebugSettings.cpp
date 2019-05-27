#ifndef PH_RELEASE

#include "collisionDebugSettings.hpp"
#include "Physics/bodyType.hpp"
#include "Utilities/debug.hpp"
#include <string>

using PopHead::Physics::CollisionDebugSettings;
using PopHead::Physics::BodyType;

CollisionDebugSettings::CollisionDebugSettings()
	:mShouldDisplay(false)
	,mShouldDisplayKinematicBodies(true)
	,mShouldDisplayStaticBodies(true)
{
	setColors(1);
}

void CollisionDebugSettings::turnOn()
{
	mShouldDisplay = true;
	PH_LOG(LogType::Info, "Collision debug was turned on.");
}

void CollisionDebugSettings::turnOff()
{
	mShouldDisplay = false;
	PH_LOG(LogType::Info, "Collision debug was turned off.");
}

void CollisionDebugSettings::displayOnlyKinematicBodies()
{
	mShouldDisplayKinematicBodies = true;
	mShouldDisplayStaticBodies = false;
	PH_LOG(LogType::Info, "Collision debug mode was changed. Now it should display only kinematic bodies.");
}

void CollisionDebugSettings::displayOnlyStaticBodies()
{
	mShouldDisplayKinematicBodies = false;
	mShouldDisplayStaticBodies = true;
	PH_LOG(LogType::Info, "Collision debug mode was changed. Now it should display only static bodies.");
}

void CollisionDebugSettings::displayAllBodies()
{
	mShouldDisplayKinematicBodies = true;
	mShouldDisplayStaticBodies = true;
	PH_LOG(LogType::Info, "Collision debug mode was changed. Now it should display all bodies.");
}


bool CollisionDebugSettings::shouldDisplay(BodyType bodyType)
{
	if (bodyType == BodyType::kinematicBody && !mShouldDisplayKinematicBodies)
		return false;
	if (bodyType == BodyType::staticBody && !mShouldDisplayStaticBodies)
		return false;

	return mShouldDisplay;
}

auto CollisionDebugSettings::getFillColor(BodyType bodyType) -> const sf::Color&
{
	switch (bodyType)
	{
	case BodyType::kinematicBody:
		return mKinematicBodiesColor;

	case BodyType::staticBody:
		return mStaticBodiesColor;
	}
}

void CollisionDebugSettings::setColors(int numberOfSet)
{
	static const sf::Color red(45, 100, 150, 140);
	static const sf::Color blue(200, 0, 0, 140);
	static const sf::Color green(24, 158, 68, 140);
	static const sf::Color orange(218, 104, 20, 140);
	static const sf::Color cyan(69, 254, 222, 140);
	static const sf::Color violet(117, 9, 90, 140);

	switch (numberOfSet)
	{
	case 1:
		mKinematicBodiesColor = red;
		mStaticBodiesColor = blue;
		break;

	case 2:
		mKinematicBodiesColor = green;
		mStaticBodiesColor = orange;
		break;

	case 3:
		mKinematicBodiesColor = cyan;
		mStaticBodiesColor = violet;
		break;

	default:
		dealWithSetColorsError(numberOfSet);
	}

	PH_LOG(LogType::Info, "Collision debug rect colors were set to " + std::to_string(numberOfSet) + ".");
}

void CollisionDebugSettings::dealWithSetColorsError(int numberOfSet)
{
	PH_EXCEPTION("You have to choose CollisionDebug color set from 1 to 3! There is no option " + std::to_string(numberOfSet) + ".");
}

#endif //!PH_RELEASE
