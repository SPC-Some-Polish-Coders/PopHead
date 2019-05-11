#ifndef PH_RELEASE

#include "collisionDebugSettings.hpp"
#include "Physics/bodyType.hpp"
#include "Logs/logger.hpp"

#include <string>
#include <stdexcept>

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

sf::Color CollisionDebugSettings::getFillColor(BodyType bodyType)
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
	switch (numberOfSet)
	{
	case 1:
		mKinematicBodiesColor = sf::Color(45, 100, 150, 135);
		mStaticBodiesColor = sf::Color(200, 0, 0, 100);
		break;

	case 2:
		mKinematicBodiesColor = sf::Color(24, 158, 68, 100);
		mStaticBodiesColor = sf::Color(218, 104, 20, 100);
		break;

	case 3:
		mKinematicBodiesColor = sf::Color(4, 134, 140, 100);
		mStaticBodiesColor = sf::Color(117, 9, 90, 100);
		break;

	default:
		dealWithSetColorsError(numberOfSet);
	}
}

void CollisionDebugSettings::dealWithSetColorsError(int numberOfSet)
{
	const std::string message = "You have to choose CollisionDebug color set from 1 to 3! There is no option "
		+ std::to_string(numberOfSet) + ".";
	PH_LOG(LogType::Error, message);
	throw std::runtime_error(message);
}

#endif //!PH_RELEASE
