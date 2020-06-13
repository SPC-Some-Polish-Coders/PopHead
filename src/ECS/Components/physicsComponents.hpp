#pragma once

#include "Utilities/rect.hpp"
#include <vector>
#include <cmath>

namespace ph {

namespace component {

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

static inline
Vec2 getCirclePos(const FloatRect& rect, component::BodyCircle circle)
{
	return rect.pos + circle.offset;
}

static inline
bool intersect(const FloatRect& a,
               const FloatRect& b, component::BodyCircle bc)
{
	return intersect(a, getCirclePos(b, bc), bc.radius);
}

static inline
bool intersect(const FloatRect& a, component::BodyCircle ac,
               const FloatRect& b, component::BodyCircle bc)
{
	auto aPos = getCirclePos(a, ac);
	auto bPos = getCirclePos(b, bc);
	return distanceBetweenPoints(aPos, bPos) < ac.radius + bc.radius;
}

static inline
bool fullyIntersect(const FloatRect& a,
                    const FloatRect& b, component::BodyCircle bc)
{
	Vec2 circlePos = getCirclePos(b, bc);
	return a.contains(circlePos) && 
	       a.contains(circlePos + Vec2(-bc.radius, 0.f)) &&
	       a.contains(circlePos + Vec2(bc.radius, 0.f)) &&
	       a.contains(circlePos + Vec2(0.f, -bc.radius)) &&
	       a.contains(circlePos + Vec2(0.f, bc.radius)); 
}

}
