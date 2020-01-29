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
						std::unique_lock<std::mutex> lock(mDataMutex);
						if (mShutdown)
							return;
						if (!mTasks.empty())
						{
							auto currentTask = std::move(mTasks.front());
							mTasks.pop();
							lock.unlock();
							currentTask();
						}
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

	std::future<void> ThreadPool::addTask(std::function<void()> task)
	{
		std::packaged_task<void()> packagedTask(task);
		std::lock_guard<std::mutex> lock(mDataMutex);
		mTasks.emplace(std::move(packagedTask));
		return mTasks.back().get_future();
	}
}
