#pragma once

#include <vector>
#include "collisionDebugRect.hpp"

namespace ph{

class CollisionBody;
enum class BodyType;

class CollisionDebugSettings
{
private:
	CollisionDebugSettings();

public:
	CollisionDebugSettings(const CollisionDebugSettings&) = delete;
	void operator=(CollisionDebugSettings&) = delete;

	static CollisionDebugSettings& getInstance()
	{
		static CollisionDebugSettings instance;
		return instance;
	}

	void turnOn();
	void turnOff();
	
	void displayOnlyKinematicBodies();
	void displayOnlyStaticBodies();
	void displayAllBodies();

	auto getFillColor(BodyType bodyType) -> const sf::Color&;
	bool shouldDisplay(BodyType);

	void setColors(int numberOfSet);
private:
	void dealWithSetColorsError(int numberOfSet);

private:
	bool mShouldDisplay;
	bool mShouldDisplayKinematicBodies;
	bool mShouldDisplayStaticBodies;
	sf::Color mKinematicBodiesColor;
	sf::Color mStaticBodiesColor;
};

}
