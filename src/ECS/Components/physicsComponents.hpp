#pragma once

#include "Utilities/rect.hpp"
#include <vector>

namespace ph::component {

	struct BodyRect
	{
		union
		{
			ph::FloatRect rect;

			struct
			{
				sf::Vector2f pos;
				sf::Vector2f size;
			};

			struct
			{
				float x, y;
				float width, height;
			};
		};
	};

	struct Velocity
	{
		sf::Vector2f d;
	};

	struct PushingForces
	{
		sf::Vector2f vel;
		float friction = 1.5f;
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
		bool staticallyMovedUp;
		bool staticallyMovedDown;
		bool staticallyMovedLeft;
		bool staticallyMovedRight;
	};
}
