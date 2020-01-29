#pragma once

#include "Events/event.hpp"

#include <entt/entity/registry.hpp>

namespace ph::system {

	class System
	{
	public:
		explicit System(entt::registry& registry) : mRegistry(registry) {}

		virtual void update(float dt) {};
		virtual void onEvent(Event) {};

		static void setPause(bool pause) { sPause = pause; }

	protected:
		entt::registry& mRegistry;
		inline static bool sPause = false;
	};
}
