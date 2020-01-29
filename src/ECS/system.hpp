#pragma once

#include "Events/event.hpp"

#include <entt/entity/registry.hpp>

namespace ph::system {

	class System
	{
	public:
		explicit System(entt::registry& registry);

		virtual void update(float seconds) {};
		virtual void onEvent(const ActionEvent& event);

		static void setPause(bool pause) { sPause = pause; }

	protected:
		entt::registry& mRegistry;
		inline static bool sPause = false;
	};
}
