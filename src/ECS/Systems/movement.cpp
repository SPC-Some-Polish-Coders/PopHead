#include "pch.hpp"
#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

using namespace component;

void Movement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<Kinematics, InsideSimRegion, BodyRect>().each([dt]
	(auto& kin, auto, auto& body)
	{
		if(kin.defaultFriction != kin.friction) 
		{
			kin.friction = lerp(kin.friction, kin.defaultFriction, kin.frictionLerpSpeed);	
			if(areApproximatelyEqual(kin.defaultFriction, kin.friction, 0.2f))
				kin.friction = kin.defaultFriction;
		}
		kin.vel += kin.acceleration * dt;
		body.pos += kin.vel * dt;
		kin.vel -= kin.vel * kin.friction * dt;
	});

	mRegistry.view<FallingIntoPit, Kinematics>().each([&]
	(auto falling, auto& kin)
	{
		kin.vel *= falling.timeToEnd / 6.f;
	});
}

}
