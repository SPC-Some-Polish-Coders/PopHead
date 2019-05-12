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
}

void CollisionDebugSettings::dealWithSetColorsError(int numberOfSet)
{
	const std::string message = "You have to choose CollisionDebug color set from 1 to 3! There is no option " +
                                 std::to_string(numberOfSet) + ".";
	PH_LOG(LogType::Error, message);
	throw std::runtime_error(message);
}

#endif //!PH_RELEASE
