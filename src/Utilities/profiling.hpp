#pragma once

#include <string>
#include <chrono>
#include <fstream>

namespace ph {

struct ProfilingResult
{
	std::string name;
	long long start, end;
	uint32_t threadID;
};

class ProfilingManager
{
private:
	ProfilingManager();
	ProfilingManager(const ProfilingManager&) = delete;
	ProfilingManager& operator=(const ProfilingManager&) = delete;

public:
	static ProfilingManager& getInstance()
	{
		static ProfilingManager instance;
		return instance;
	}

	void beginSession(const std::string& name, const std::string& filepath = "results.json");
	void endSession();

	void writeProfile(const ProfilingResult& result);
	void writeHeader();
	void writeFooter();

private:
	std::ofstream mOutputStream;
	int mProfileCount;
	bool mIsThereActiveSession;
};

class ProfilingTimer
{
public:
	ProfilingTimer(const char* name);
	~ProfilingTimer();

	void stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTimepoint;
	const char* mName;
	bool mStopped;
};

}

#define PH_PROFILING 1 
#if PH_PROFILING
	#define PH_BEGIN_PROFILING_SESSION(name, filepath) ph::ProfilingManager::getInstance().beginSession(name, filepath)
	#define PH_END_PROFILING_SESSION() ph::ProfilingManager::getInstance().endSession()
	#define PH_PROFILE_SCOPE(name) ph::ProfilingTimer profTimer##__LINE__(name);
	#define PH_PROFILE_FUNCTION() PH_PROFILE_SCOPE(__FUNCTION__);
#else
	#define PH_BEGIN_PROFILING_SESSION(name, filepath)
	#define PH_END_PROFILING_SESSION()
	#define PH_PROFILE_SCOPE(name)
	#define PH_PROFILE_FUNCTION()
#endif
