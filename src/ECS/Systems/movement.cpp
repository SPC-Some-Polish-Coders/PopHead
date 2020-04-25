#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"

namespace ph::system {
	
	void Movement::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		auto bodiesWithVel = mRegistry.view<component::BodyRect, component::Kinematics>();
		bodiesWithVel.each([dt](component::BodyRect& body, component::Kinematics& kin) 
		{
			if(kin.defaultFriction != kin.friction) 
			{
				kin.friction = Math::lerp(kin.friction, kin.defaultFriction, kin.frictionLerpSpeed);	
				if(Math::areApproximatelyEqual(kin.defaultFriction, kin.friction, 0.2f))
				{
					kin.friction = kin.defaultFriction;
				}
			}
			kin.vel += kin.acceleration * dt;
			body.pos += kin.vel * dt;
			kin.vel -= kin.vel * kin.friction * dt;
		});
	}
}
