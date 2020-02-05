#pragma once

#include "system.hpp"

#include <vector>
#include <memory>

namespace ph {

	class ThreadPool;

	class SystemsQueue
	{
	public:
		SystemsQueue(entt::registry& registry, ThreadPool&);

		void update(float seconds);
		void handleEvents(Event);
	
		template <typename SystemType, typename... Args>
		void appendSystem(Args... arguments);

	private:
		entt::registry& mRegistry;
		ThreadPool& mThreadPool;
		std::vector<std::unique_ptr<system::System>> mSystemsArray;
	};
}

#include "systemsQueue.inl"
