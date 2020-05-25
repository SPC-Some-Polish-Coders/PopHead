#pragma once

#include "Utilities/rect.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>
#include <array>

namespace ph::component {

	struct ArcadeSpawner // @no-debugger
	{
		struct Wave
		{
			unsigned normalZombiesToSpawn;
			unsigned slowZombiesToSpawn;
		};
		std::array<Wave, 10> waves;
		float timeFromLastSpawn = 0.5f;
	};

	struct LootSpawner // @no-debugger
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
		std::string keyboardContent;
		std::string joystickContent;
		bool isShown;
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

	enum class PuzzleColor
	{
		Grey, Red, Green, Blue 
	};

	struct PressurePlate
	{
		unsigned puzzleId;
		unsigned id;
		PuzzleColor pressedByColor;
		bool isPressed = false;
		bool isPressIrreversible;
	};

	struct PuzzleBoulder
	{	
		float pushedLeftSince = 0.f;
		float pushedRightSince = 0.f;
		float pushedUpSince = 0.f;
		float pushedDownSince = 0.f;
		float movingLeft = 0.f;
		float movingRight = 0.f;
		float movingUp = 0.f;
		float movingDown = 0.f;
		bool movedGridPosInThisMove = false;
	};

	struct PuzzleGridPos : public sf::Vector2i 
	{
		using sf::Vector2i::operator=;
	};

	struct PuzzleGridRoadChunk
	{
		bool tiles[12][12] = {}; // 1 - road, 0 - collision
	};

	struct Gate
	{
		unsigned id;
		bool previouslyOpen = false; 
		bool open = false;
	};

	struct Spikes 
	{
		unsigned puzzleId;
		unsigned id;
		float timeToChange; 
		float changeFrequency;
		bool changes;
		bool active;
	};

	struct GunProperties // @no-debugger
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

	struct MeleeProperties // @no-debugger
	{
		float minHitInterval;
		float rotationRange;
		float rotationSpeed;
		float range;
		int damage;
	};

	struct Weather {}; // @no-debugger

	struct SavePoint 
	{
		bool isIntersectingPlayer = false;
		float timeSincePlayerSteppedOnIt = 2.f;
	};

}
