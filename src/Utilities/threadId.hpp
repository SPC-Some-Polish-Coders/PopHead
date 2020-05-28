#pragma once

#include <thread>
#include <mutex>
#include <map>

namespace ph {

class ThreadId {
	ThreadId() = delete;

public:
	static size_t getCurrentThreadId();

private:
	static size_t createFriendlyId();

private:
	static std::map<std::thread::id, size_t> mHardwareToFriendlyId;
	static std::mutex mDataMutex;
	static thread_local i32 mCurrentFriendlyId;
};
}
