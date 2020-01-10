#include "profiling.hpp"
#include <algorithm>
#include <thread>

namespace ph {

ProfilingManager::ProfilingManager()
	:mIsThereActiveSession(false)
	,mProfileCount(0)
{
}

void ProfilingManager::beginSession(const std::string& name, const std::string& filepath)
{
	mOutputStream.open(filepath);
	writeHeader();
	mIsThereActiveSession = true;
}

void ProfilingManager::endSession()
{
	writeFooter();
	mOutputStream.close();
	mIsThereActiveSession = false;
	mProfileCount = 0;
}

void ProfilingManager::writeProfile(std::string&& name, long long start, long long end, unsigned threadID)
{
	if(!mIsThereActiveSession)
		return;

	if(mProfileCount++ > 0)
		mOutputStream << ",";

	std::replace(name.begin(), name.end(), '"', '\'');

	mOutputStream << "{";
	mOutputStream << "\"cat\":\"function\",";
	mOutputStream << "\"dur\":" << (end - start) << ',';
	mOutputStream << "\"name\":\"" << name << "\",";
	mOutputStream << "\"ph\":\"X\",";
	mOutputStream << "\"pid\":0,";
	mOutputStream << "\"tid\":" << threadID << ",";
	mOutputStream << "\"ts\":" << start;
	mOutputStream << "}";
}

void ProfilingManager::writeHeader()
{
	mOutputStream << "{\"otherData\": {},\"traceEvents\":[";
}

void ProfilingManager::writeFooter()
{
	mOutputStream << "]}";
	mOutputStream.flush();
}

ProfilingTimer::ProfilingTimer(const char* name, unsigned threadID)
	:mName(name)
	,mThreadID(threadID)
	,mStopped(false)
{
	mStartTimepoint = std::chrono::high_resolution_clock::now();
}

ProfilingTimer::~ProfilingTimer()
{
	if(!mStopped)
		stop();
}

void ProfilingTimer::stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimepoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
	ProfilingManager::getInstance().writeProfile(std::move(mName), start, end, mThreadID);
	mStopped = true;
}

}

