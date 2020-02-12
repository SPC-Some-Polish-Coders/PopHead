#pragma once

#include <entt/entity/registry.hpp>
#include <SFML/Window/Event.hpp>

namespace ph::system {

	class System
	{
	public:
		explicit System(entt::registry& registry) : mRegistry(registry) {}

		virtual void update(float dt) {};
		virtual void onEvent(sf::Event) {};

		static void setPause(bool pause) { sPause = pause; }

	protected:
		entt::registry& mRegistry;
		inline static bool sPause = false;
	};
}
