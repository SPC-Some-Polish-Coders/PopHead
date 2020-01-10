#include "velocityClear.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void VelocityClear::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		auto view = mRegistry.view<component::Velocity>();

		view.each([dt](component::Velocity& vel) {
			vel.dx = 0.f;
			vel.dy = 0.f;
		});
	}
}
