#pragma once

#include "Utilities/rect.hpp"

namespace ph::component {

	struct Area
	{
		FloatRect areaBody;
	};

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
		float minActivationInterval;
		float activationCooldown;
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
		float minShotsInterval;
		float range;
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
