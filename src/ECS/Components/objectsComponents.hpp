#pragma once

#include "Utilities/rect.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>

namespace ph::component {

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

struct Puzzle 
{
	u8 id;
};

struct PuzzleId
{
	union
	{
		struct
		{
			u8 puzzleId;
			u8 elementId;
		};
		u16 hash;
	};
};

enum class PuzzleColor
{
	Grey, Red, Green, Blue 
};

struct Lever
{
	bool active = false;
	bool wasJustSwitched = false;
	bool turnOffAfterSwitch = false;
};

struct PressurePlate
{
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
	u16 id;
	bool previouslyOpen = false; 
	bool open = false;
};

struct Spikes 
{
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
	i16 damage;
	u16 numberOfBullets;
	u8 gunId;

	enum class Type { Pistol, Shotgun };
	Type type;
};

struct MeleeProperties // @no-debugger
{
	float minHitInterval;
	float rotationRange;
	float rotationSpeed;
	float range;
	i16 damage;
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
