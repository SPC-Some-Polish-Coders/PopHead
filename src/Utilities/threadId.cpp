#include "threadId.hpp"

namespace ph {

std::map<std::thread::id, size_t> ThreadId::mHardwareToFriendlyId;
std::mutex ThreadId::mDataMutex;
thread_local auto currentFriendlyId = -1;

size_t ThreadId::getCurrentThreadId()
{
	// friendly id will be created only for threads that ask for it
	if (currentFriendlyId == -1)
		currentFriendlyId = createFriendlyId();
	return currentFriendlyId;
}

size_t ThreadId::createFriendlyId()
{
	std::lock_guard lock(mDataMutex);
	auto newFriendlyId = ThreadId::mHardwareToFriendlyId.size();
	auto pair = ThreadId::mHardwareToFriendlyId.emplace(std::this_thread::get_id(), newFriendlyId);
	return pair.first->second;
}
}
