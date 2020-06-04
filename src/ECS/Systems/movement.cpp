#include "pch.hpp"
#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

using namespace component;

void Movement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<BodyRect, Kinematics>().each([dt]
	(auto& body, auto& kin)
	{
		if(kin.defaultFriction != kin.friction) 
		{
			kin.friction = Math::lerp(kin.friction, kin.defaultFriction, kin.frictionLerpSpeed);	
			if(Math::areApproximatelyEqual(kin.defaultFriction, kin.friction, 0.2f))
				kin.friction = kin.defaultFriction;
		}
		kin.vel += kin.acceleration * dt;
		body.pos += kin.vel * dt;
		kin.vel -= kin.vel * kin.friction * dt;
	});
}

}
