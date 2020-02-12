#pragma once

#include "system.hpp"

#include <vector>
#include <memory>

namespace ph {

	class SystemsQueue
	{
	public:
		SystemsQueue(entt::registry& registry);

		void update(float seconds);
		void handleEvents(sf::Event);
	
		template <typename SystemType, typename... Args>
		void appendSystem(Args... arguments);

	private:
		entt::registry& mRegistry;
		std::vector<std::unique_ptr<system::System>> mSystemsArray;
	};
}

#include "systemsQueue.inl"
