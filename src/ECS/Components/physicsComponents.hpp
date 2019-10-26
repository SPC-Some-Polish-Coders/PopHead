#pragma once

#include "Utilities/rect.hpp"

namespace ph::component {

	struct Position
	{
		float x, y;
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
		ph::FloatRect body;
	};
	
	struct KinematicCollisionBody 
	{
		ph::FloatRect body;
		float mass;
	};
}
