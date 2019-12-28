#include "pushingMovement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void PushingMovement::update(float dt)
	{
		auto view = mRegistry.view<component::PushingVelocity, component::BodyRect>();
		view.each([dt](component::PushingVelocity& pushingVel, component::BodyRect& body)
		{
			body.rect.move(pushingVel.vel);

			pushingVel.vel -= pushingVel.vel * dt;
			if(pushingVel.vel.x < 1 && pushingVel.vel.x > -1 && pushingVel.vel.y < 1 && pushingVel.vel.y > -1)
				pushingVel.vel = sf::Vector2f(0, 0);
		});
	}
}

