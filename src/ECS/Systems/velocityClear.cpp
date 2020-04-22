#include "velocityClear.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

	void VelocityClear::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		auto view = mRegistry.view<component::Velocity>();
		view.each([dt](component::Velocity& vel) {
			vel.d = sf::Vector2f(0, 0);
		});
	}
}
