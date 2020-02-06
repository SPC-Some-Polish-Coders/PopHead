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
		struct QueueItem
		{
			QueueItem(std::unique_ptr<system::System>&& s, size_t o)
				: order(o)
			{
				system.swap(s);
			}
			std::unique_ptr<system::System> system;
			size_t order;
		};

		entt::registry& mRegistry;
		ThreadPool& mThreadPool;
		std::vector<QueueItem> mSystemsArray;
	};
}

#include "systemsQueue.inl"
