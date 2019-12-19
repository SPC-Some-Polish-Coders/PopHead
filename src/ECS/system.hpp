#pragma once

#include "Events/event.hpp"

#include <entt/entity/registry.hpp>

namespace ph::system {

	class System
	{
	public:
		explicit System(entt::registry& registry);

		virtual void update(float seconds) = 0;
		virtual void onEvent(const ActionEvent& event);

	protected:
		entt::registry& mRegistry;
	};
}
