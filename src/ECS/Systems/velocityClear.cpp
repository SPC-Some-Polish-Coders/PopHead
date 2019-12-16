#include "velocityClear.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

	void VelocityClear::update(float dt)
	{
		auto view = mRegistry.view<component::Velocity>();

		view.each([dt](component::Velocity& vel) {
			vel.dx = 0.f;
			vel.dy = 0.f;
		});
	}
}
