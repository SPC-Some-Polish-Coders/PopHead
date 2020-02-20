#pragma once

#include <string>
#include <chrono>
#include <fstream>
#include <mutex>

namespace ph {

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

	void writeProfile(std::string&& profilingScopeName, long long start, long long end, unsigned threadID);
	void writeHeader();
	void writeFooter();

private:
	std::ofstream mOutputStream;
	int mProfileCount;
	bool mIsThereActiveSession;
	std::mutex mDataMutex;
};

class ProfilingTimer
{
public:
	ProfilingTimer(const char* name, unsigned threadID);
	~ProfilingTimer();

	void stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTimepoint;
	const char* mName;
	unsigned mThreadID;
	bool mStopped;
};

}

#define PH_PROFILING 0 
#if PH_PROFILING
	#if _MSC_VER 
		#pragma message ("POPHEAD WARNING: Make sure to disable PH_PROFILING before making commit!")
	#endif
	#define PH_BEGIN_PROFILING_SESSION(name, filepath) ph::ProfilingManager::getInstance().beginSession(name, filepath)
	#define PH_END_PROFILING_SESSION() ph::ProfilingManager::getInstance().endSession()
	#define PH_PROFILE_SCOPE(name, threadID) ph::ProfilingTimer profTimer##__LINE__(name, threadID);
	#define PH_PROFILE_FUNCTION(threadID) PH_PROFILE_SCOPE(__FUNCTION__, threadID);
#else
	#define PH_BEGIN_PROFILING_SESSION(name, filepath)
	#define PH_END_PROFILING_SESSION()
	#define PH_PROFILE_SCOPE(name, threadID)
	#define PH_PROFILE_FUNCTION(threadID)
#endif

// Make sure to not commit calls of these macros! They should be used to measure performance of some small piece of code.
#define PH_BEGIN_PROFILING_LOCAL_SESSION(name, filepath) ph::ProfilingManager::getInstance().beginSession(name, filepath)
#define PH_END_PROFILING_LOCAL_SESSION() ph::ProfilingManager::getInstance().endSession()
#define PH_PROFILE_SCOPE_LOCAL_SESSION(name, threadID) ph::ProfilingTimer profLocalTimer##__LINE__(name, threadID);
#define PH_PROFILE_FUNCTION_LOCAL_SESSION(threadID) PH_PROFILE_SCOPE(__FUNCTION__, threadID);

