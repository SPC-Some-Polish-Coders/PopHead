#include "system.hpp"

namespace ph::system {
		
	System::System(entt::registry& registry)
		: mRegistry(registry)
	{
	}

	void System::onEvent(const ActionEvent& event)
	{
	}

}
