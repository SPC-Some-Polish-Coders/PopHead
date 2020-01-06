#include "pushingMovement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void PushingMovement::update(float dt)
	{
		auto view = mRegistry.view<component::PushingForces, component::KinematicCollisionBody, component::BodyRect>();
		view.each([dt](component::PushingForces& pf, const component::KinematicCollisionBody kinematicCollisionBody, component::BodyRect& body)
		{
			// move body
			body.rect.move(pf.vel);

			// decrese velocity of body or stop body
			pf.vel -=  pf.vel * pf.friction * dt;
			if(pf.vel.x < 1 && pf.vel.x > -1 && pf.vel.y < 1 && pf.vel.y > -1)
				pf.vel = sf::Vector2f(0, 0);
			/*if(kinematicCollisionBody.staticallyMovedByX)
				pf.vel.x = 0.f;
			if(kinematicCollisionBody.staticallyMovedByY)
				pf.vel.y = 0.f;*/
		});
	}
}

