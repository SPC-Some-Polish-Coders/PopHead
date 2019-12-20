#pragma once

#include "Utilities/rect.hpp"
#include <vector>

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

	struct MultiStaticCollisionBody
	{
		std::vector<ph::FloatRect> rects;
	};
	
	struct KinematicCollisionBody 
	{
		float mass;
		bool staticallyMovedByX;
		bool staticallyMovedByY;
	};
}
