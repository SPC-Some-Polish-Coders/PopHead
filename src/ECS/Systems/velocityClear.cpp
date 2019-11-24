#include "velocityClear.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void VelocityClear::update(float seconds)
	{
		auto view = mRegistry.view<component::Velocity>();

		view.each([seconds](component::Velocity& vel) {
			vel.dx = 0.f;
			vel.dy = 0.f;
			});
	}
}
