#pragma once

#include <thread>
#include <future>
#include <mutex>
#include <queue>
#include <vector>

namespace ph {

	class ThreadPool
	{
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;

	public:
		ThreadPool(size_t numOfThreads);
		~ThreadPool();

		std::future<void> addTask(std::function<void()> task);

	private:
		std::vector<std::thread> mThreads;
		bool mShutdown = false;
		std::mutex mDataMutex;
		std::queue<std::packaged_task<void()>> mTasks;
	};
}
