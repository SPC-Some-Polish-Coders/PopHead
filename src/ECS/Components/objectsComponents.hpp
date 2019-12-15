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

	struct Entrance
	{
		std::string entranceDestination;
		sf::Vector2f playerSpawnPosition;
	};

	struct Lever
	{
		bool isActivated;
		float minActivationInterval;
		float activationCooldown;
	};

	struct Gate
	{
		bool isOpened;
	};

	struct LeverListener
	{
		bool isActivated;
		sf::Vector2f leverPosition;
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
