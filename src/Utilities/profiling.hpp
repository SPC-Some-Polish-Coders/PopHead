#pragma once

#include <SFML/System/Clock.hpp>
#include <string>
#include <chrono>
#include <fstream>
#include <mutex>
#include <vector>

namespace ph {

struct ProfilingResult
{
	using id = unsigned long;

	id resultId;
	std::string name;
	long long startTime;
	unsigned int duration;
	std::vector<std::pair<std::string, std::string>> args;
};

class MainProfilingManager
{
	class ThreadProfilingManager;
	MainProfilingManager() = delete;
public:
	static ProfilingResult::id commitResultStart(std::string name, std::vector<std::pair<std::string, std::string>> args);
	static void commitResultEnd(ProfilingResult::id id);

	static void beginSession(const std::string& filepath = "results.json");
	static void endSession();

private:
	static thread_local ThreadProfilingManager mThreadManager;
	static std::ofstream mOutputFile;
	static std::mutex mFileMutex;
	static bool mIsActive;
};

class MainProfilingManager::ThreadProfilingManager
{
public:
	ThreadProfilingManager() = default;

	ProfilingResult::id commitResultStart(std::string&& name, std::vector<std::pair<std::string, std::string>>&& args);
	void commitResultEnd(ProfilingResult::id id);

	bool hasCommitedResults() const;
	std::vector<ProfilingResult> getCommitedResults();

private:
	ProfilingResult::id mNextResultId = 0;
	std::vector<ProfilingResult> mResults;
	bool mIsFirstResultFinished = false;
};

class ProfilingTimer
{
public:
	ProfilingTimer(const char* name, std::vector<std::pair<std::string, std::string>> args);
	~ProfilingTimer();

private:
	ProfilingResult::id resultId;
};

struct ImGuiProfilingResult
{
	std::string name;
	float duration; // microseconds
};

class ImGuiProfilingTimer
{
public:
	ImGuiProfilingTimer(const char* name);
	~ImGuiProfilingTimer();

private:
	ImGuiProfilingResult mResult;
	sf::Clock clock;
};

namespace ImGuiProfiling
{
	void commitResult(const ImGuiProfilingResult&);
	void flush(float dt);
}

}

#if defined(PH_PROFILING)
	#define PH_BEGIN_PROFILING_SESSION(filepath) ph::MainProfilingManager::beginSession(filepath);
	#define PH_END_PROFILING_SESSION() ph::MainProfilingManager::endSession();

	#define PH_PROFILE_SCOPE(name) ph::ProfilingTimer profTimer##__LINE__(name, {}); ph::ImGuiProfilingTimer imguiProfTimer##__LINE__(name);
	#define PH_PROFILE_SCOPE_ARGS(name, ...) ph::ProfilingTimer profTimer##__LINE__(name, __VA_ARGS__); ph::ImGuiProfilingTimer imguiProfTimer##__LINE__(name);

	#define PH_PROFILE_FUNCTION() PH_PROFILE_SCOPE(__FUNCTION__);
	#define PH_PROFILE_FUNCTION_ARGS(...) PH_PROFILE_SCOPE_ARGS(__FUNCTION__, __VA_ARGS__);
#elif !defined(PH_DISTRIBUTION)
	#define PH_BEGIN_PROFILING_SESSION(filepath)
	#define PH_END_PROFILING_SESSION()

	#define PH_PROFILE_SCOPE(name) ph::ImGuiProfilingTimer imguiProfTimer##__LINE__(name);
	#define PH_PROFILE_SCOPE_ARGS(name, ...) ph::ImGuiProfilingTimer imguiProfTimer##__LINE__(name);

	#define PH_PROFILE_FUNCTION() PH_PROFILE_SCOPE(__FUNCTION__);
	#define PH_PROFILE_FUNCTION_ARGS(...) PH_PROFILE_SCOPE_ARGS(__FUNCTION__, __VA_ARGS__);
#else
	#define PH_BEGIN_PROFILING_SESSION(filepath)
	#define PH_END_PROFILING_SESSION()

	#define PH_PROFILE_SCOPE(name)
	#define PH_PROFILE_SCOPE_ARGS(name, ...)

	#define PH_PROFILE_FUNCTION()
	#define PH_PROFILE_FUNCTION_ARGS(...)
#endif
