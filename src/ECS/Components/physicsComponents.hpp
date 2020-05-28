#pragma once

#include "Utilities/rect.hpp"
#include <vector>

namespace ph::component {

	struct BodyRect : public FloatRect 
	{
		using FloatRect::operator=;
	};

	struct BodyCircle
	{
		Vec2 offset;
		float radius;
	};

	struct Kinematics
	{
		Vec2 vel;
		Vec2 acceleration;
		float friction;
		float defaultFriction;
		float frictionLerpSpeed;
	};

	struct CharacterSpeed
	{
		float speed;
	};

	struct StaticCollisionBody {};

	struct MultiStaticCollisionBody
	{
		std::vector<FloatRect> rects;
		std::vector<BodyCircle> circles;
		FloatRect sharedBounds;
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
