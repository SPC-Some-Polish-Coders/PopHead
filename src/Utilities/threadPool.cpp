#include "threadPool.hpp"

namespace ph {
	ThreadPool::ThreadPool(size_t numOfThreads)
	{
		for (size_t i = 0; i < numOfThreads; ++i)
		{
			mThreads.emplace_back([this]()
				{
					while (true)
					{
						std::lock_guard<std::mutex> lock(mDataMutex);
						if (mShutdown)
							return;
					}
				});
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::lock_guard<std::mutex> lock(mDataMutex);
			mShutdown = true;
		}
		
		for (auto& thread : mThreads)
		{
			thread.join();
		}
	}
}
