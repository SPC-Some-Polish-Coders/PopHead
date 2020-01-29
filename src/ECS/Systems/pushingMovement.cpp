#include "pushingMovement.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void PushingMovement::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		if(sPause)
			return;

		auto view = mRegistry.view<component::PushingForces, component::KinematicCollisionBody, component::BodyRect>();
		view.each([dt](component::PushingForces& pf, const component::KinematicCollisionBody kinematicCollisionBody, component::BodyRect& body)
		{
			// move body
			body.rect.move(pf.vel);

			// decrese velocity of body or stop body
			pf.vel -=  pf.vel * pf.friction * dt;
			if(pf.vel.x < 1 && pf.vel.x > -1 && pf.vel.y < 1 && pf.vel.y > -1)
				pf.vel = sf::Vector2f(0, 0);
		});
	}
}
