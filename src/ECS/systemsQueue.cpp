#include "systemsQueue.hpp"
#include "Utilities/threadPool.hpp"

namespace ph {
	
	SystemsQueue::SystemsQueue(entt::registry& registry, ThreadPool& threadPool)
		: mRegistry(registry)
		, mThreadPool(threadPool)
	{
	}

	void SystemsQueue::update(float seconds)
	{
		size_t currentOrder = 0;
		auto itemIter = mSystemsArray.begin();
		std::vector<std::unique_ptr<system::System>*> currentSystems;
		std::vector<std::future<void>> tasks;

		while (true)
		{
			if (itemIter == mSystemsArray.end() || itemIter->order != currentOrder)
			{
				for (size_t i = 1; i < currentSystems.size(); ++i)
				{
					tasks.emplace_back(mThreadPool.addTask([seconds, &currentSystems, i] { currentSystems[i]->get()->update(seconds); }));
				}

				if (!currentSystems.empty())
					currentSystems[0]->get()->update(seconds);

				for (auto& task : tasks)
					task.get();

				currentSystems.clear();
				tasks.clear();
				++currentOrder;
			}

			if (itemIter == mSystemsArray.end())
				break;

			currentSystems.push_back(&itemIter->system);
			++itemIter;
		}
	}

	void SystemsQueue::handleEvents(sf::Event e)
	{
		for (auto& item : mSystemsArray)
			item.system->onEvent(e);
	}

}
