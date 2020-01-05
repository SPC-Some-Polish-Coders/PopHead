#include "pushingMovement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void PushingMovement::update(float dt)
	{
		auto view = mRegistry.view<component::PushingForces, component::KinematicCollisionBody, component::BodyRect>();
		view.each([dt](component::PushingForces& pushingVel, const component::KinematicCollisionBody kinematicCollisionBody, component::BodyRect& body)
		{
			// move body
			body.rect.move(pushingVel.vel);

			// decrese velocity of body or stop body
			pushingVel.vel -=  pushingVel.vel * pushingVel.friction * dt;
			if(pushingVel.vel.x < 1 && pushingVel.vel.x > -1 && pushingVel.vel.y < 1 && pushingVel.vel.y > -1)
				pushingVel.vel = sf::Vector2f(0, 0);
			/*if(kinematicCollisionBody.staticallyMovedByX)
				pushingVel.vel.x = 0.f;
			if(kinematicCollisionBody.staticallyMovedByY)
				pushingVel.vel.y = 0.f;*/
		});
	}
}

