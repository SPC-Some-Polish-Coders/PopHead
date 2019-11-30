#pragma once

#include "Utilities/rect.hpp"

namespace ph::component {

	struct BodyRect
	{
		ph::FloatRect rect;
	};

	struct Velocity
	{
		float dx, dy;
	};

	struct CharacterSpeed
	{
		float speed;
	};

	struct StaticCollisionBody
	{
	};

	struct GateState
	{
		bool open;
	};
	
	struct KinematicCollisionBody 
	{
		float mass;
		bool staticallyMovedByX;
		bool staticallyMovedByY;
	};
}
