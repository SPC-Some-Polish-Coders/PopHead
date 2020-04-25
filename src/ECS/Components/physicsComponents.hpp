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

	struct Kinematics
	{
		sf::Vector2f vel;
		sf::Vector2f acceleration;
		float friction;
		float defaultFriction;
		float frictionLerpSpeed;
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
