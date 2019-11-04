#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {
	
	void Movement::update(float seconds)
	{
		auto view = mRegistry.view<component::BodyRect, component::Velocity>();

		view.each([seconds](component::BodyRect& body, const component::Velocity& vel) {
			body.rect.left += vel.dx * seconds;
			body.rect.top  += vel.dy * seconds;
			});
	}
}
