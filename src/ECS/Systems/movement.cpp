#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {
	
	void Movement::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto bodiesWithVel = mRegistry.view<component::BodyRect, component::Velocity>(entt::exclude<component::PushingVelocity>);
		bodiesWithVel.each([dt](component::BodyRect& body, const component::Velocity& vel) {
			body.rect.left += vel.dx * dt;
			body.rect.top  += vel.dy * dt;
		});

		auto bodiesWithVelAndPushingVel = mRegistry.view<component::BodyRect, component::Velocity, component::PushingVelocity>();
		bodiesWithVelAndPushingVel.each([dt](component::BodyRect& body, const component::Velocity& vel, const component::PushingVelocity& pushingVel) {
			if(pushingVel.vel == sf::Vector2f(0, 0)) {
				body.rect.left += vel.dx * dt;
				body.rect.top  += vel.dy * dt;
			}
		});
	}
}
