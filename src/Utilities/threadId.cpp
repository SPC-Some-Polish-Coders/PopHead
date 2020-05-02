#include "pch.hpp"
#include "threadId.hpp"

namespace ph {

std::map<std::thread::id, size_t> ThreadId::mHardwareToFriendlyId;
std::mutex ThreadId::mDataMutex;
thread_local int ThreadId::mCurrentFriendlyId = -1;

size_t ThreadId::getCurrentThreadId()
{
	// friendly id will be created only for threads that ask for it
	if (mCurrentFriendlyId == -1)
		mCurrentFriendlyId = static_cast<int>(createFriendlyId());
	return mCurrentFriendlyId;
}

size_t ThreadId::createFriendlyId()
{
	std::lock_guard lock(mDataMutex);
	auto newFriendlyId = ThreadId::mHardwareToFriendlyId.size();
	auto pair = ThreadId::mHardwareToFriendlyId.emplace(std::this_thread::get_id(), newFriendlyId);
	return pair.first->second;
}

}
