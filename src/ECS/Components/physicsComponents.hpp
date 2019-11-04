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
	
	struct KinematicCollisionBody 
	{
		float mass;
	};
}
