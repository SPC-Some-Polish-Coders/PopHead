#pragma once

#include "Utilities/rect.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>

namespace ph::component {

	struct AreaVelocityChangingEffect
	{
		float areaSpeedMultiplier;
	};

	struct PushingArea
	{
		sf::Vector2f pushForce;
	};

	struct Entrance
	{
		std::string entranceDestination;
		sf::Vector2f playerSpawnPosition;
	};

	struct Lever
	{
		unsigned id;
		bool isActivated;
		bool turnOffAfterSwitch;
	};

	struct LeverListener
	{
		unsigned observedLeverId;
		bool isActivated;
	};

	struct Gate
	{
		bool isOpened;
	};

	struct GunProperties
	{
		std::string shotSoundFilepath;
		float minShotsInterval;
		float range;
		float deflectionAngle;
		int damage;
		int numberOfBullets;
	};

	struct MeleeProperties
	{
		float minHitInterval;
		float rotationRange;
		float rotationSpeed;
		float range;
		int damage;
	};
}
