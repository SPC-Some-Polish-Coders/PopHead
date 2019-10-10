#include "movement.hpp"

#include "ECS/components.hpp"

void ph::Movement::update(float seconds)
{
	auto view = mRegistry.view<Position, Velocity>();

	view.each([seconds](Position& pos, const Velocity& vel) {
		pos.x += vel.dx * seconds;
		pos.y += vel.dy * seconds;
		});
}
