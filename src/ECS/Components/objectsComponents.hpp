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
		u32 normalZombiesToSpawn;
		u32 slowZombiesToSpawn;
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
	Vec2 pushForce;
};

struct Lever
{
	u32 id = 0;
	u32 puzzleId = 0;
	bool active = false;
	bool wasJustSwitched = false;
	bool turnOffAfterSwitch = false;
};

enum class PuzzleColor
{
	Grey, Red, Green, Blue 
};

struct PressurePlate
{
	u32 puzzleId;
	u32 id;
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

struct PuzzleGridPos : public Vec2i 
{
	using Vec2i::operator=;
};

// TODO: store this as bit flags
struct PuzzleGridRoadChunk
{
	bool tiles[12][12] = {}; // 1 - road, 0 - collision
};

struct PitChunk 
{
	std::vector<FloatRect> pits;
};

struct Gate
{
	u32 id;
	bool previouslyOpen = false; 
	bool open = false;
};

struct Spikes 
{
	u32 puzzleId;
	u32 id;
	float timeToChange; 
	float changeFrequency;
	bool changes;
	bool active;
};

struct MovingPlatform
{
	FloatRect pathBody;
	Vec2 fullVelocity;
	Vec2 currentVelocity;
	float pathCompletion;
	bool active;
	u32 puzzleId;
	u32 id;
};

struct FallingPlatform
{
	enum State
	{
		isStable,
		isFallingApart,
		isRecovering
	};
	State state;
	float timeToChangeState;
	float timeToChangeAnimationFrame;
};

struct GunProperties // @no-debugger
{
	std::string shotSoundFilepath;
	float range;
	float deflectionAngle;
	i32 damage;
	i32 numberOfBullets;
	u32 gunId;

	enum class Type { Pistol, Shotgun };
	Type type;
};

struct MeleeProperties // @no-debugger
{
	float minHitInterval;
	float rotationRange;
	float rotationSpeed;
	float range;
	i32 damage;
};

struct Weather {}; // @no-debugger

struct WeatherArea {};

enum class WeatherType
{
	Sunny, 
	Cave, 
	DrizzleRain,
	NormalRain,
	HeavyRain
};

struct SavePoint 
{
	bool isIntersectingPlayer = false;
	float timeSincePlayerSteppedOnIt = 2.f;
};

}
