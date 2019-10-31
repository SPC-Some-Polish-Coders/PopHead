#include "movement.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {
	
	void Movement::update(float seconds)
	{
		auto view = mRegistry.view<component::Position, component::Velocity>();

		view.each([seconds](component::Position& pos, const component::Velocity& vel) {
			pos.x += vel.dx * seconds;
			pos.y += vel.dy * seconds;
			});
	}
}
