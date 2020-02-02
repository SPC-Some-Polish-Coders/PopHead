#pragma once

#include "Utilities/rect.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>
#include <array>

namespace ph::component {

	struct ArcadeSpawner
	{
		struct Wave
		{
			unsigned normalZombiesToSpawn;
			unsigned slowZombiesToSpawn;
		};
		std::array<Wave, 10> waves;
		float timeFromLastSpawn = 0.5f;
	};

	struct LootSpawner
	{
		enum LootType { Bullets, Medkit };
		LootType type;
	};

	struct AreaVelocityChangingEffect
	{
		float areaSpeedMultiplier;
	};

	struct Hint
	{
		std::string hintName;
		std::string content;
		bool isShown;
	};

	struct PushingArea
	{
		sf::Vector2f pushForce;
	};

	struct CutScene
	{
		std::string name;
		bool isStartingCutSceneOnThisMap;
		bool wasActivated = false;
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

	struct Car
	{
		sf::Vector2f direction;
		float velocity;
		float acceleration;
		float slowingDown;
		bool shouldSpeedUp = false;
		bool shouldSlowDown = false;
	};

	struct GunProperties
	{
		std::string shotSoundFilepath;
		float range;
		float deflectionAngle;
		int damage;
		int numberOfBullets;
		unsigned gunId;

		enum class Type { Pistol, Shotgun };
		Type type;
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
