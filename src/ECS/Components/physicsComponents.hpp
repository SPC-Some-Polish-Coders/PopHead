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

	struct PushingVelocity
	{
		sf::Vector2f vel;
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
		ph::FloatRect sharedBounds;
	};
	
	struct KinematicCollisionBody 
	{
		float mass;
		bool staticallyMovedByX;
		bool staticallyMovedByY;
	};
}
