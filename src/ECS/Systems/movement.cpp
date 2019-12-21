#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {
	
	void Movement::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		auto view = mRegistry.view<component::BodyRect, component::Velocity>();

		view.each([dt](component::BodyRect& body, const component::Velocity& vel) {
			body.rect.left += vel.dx * dt;
			body.rect.top  += vel.dy * dt;
		});
	}
}
