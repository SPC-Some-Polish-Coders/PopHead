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

void ProfilingManager::writeProfile(const ProfilingResult& result)
{
	if(!mIsThereActiveSession)
		return;

	if(mProfileCount++ > 0)
		mOutputStream << ",";

	std::string name = result.name;
	std::replace(name.begin(), name.end(), '"', '\'');

	mOutputStream << "{";
	mOutputStream << "\"cat\":\"function\",";
	mOutputStream << "\"dur\":" << (result.end - result.start) << ',';
	mOutputStream << "\"name\":\"" << name << "\",";
	mOutputStream << "\"ph\":\"X\",";
	mOutputStream << "\"pid\":0,";
	mOutputStream << "\"tid\":" << result.threadID << ",";
	mOutputStream << "\"ts\":" << result.start;
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

ProfilingTimer::ProfilingTimer(const char* name)
	:mName(name)
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

	uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	ProfilingManager::getInstance().writeProfile({mName, start, end, threadID});

	mStopped = true;
}

}
