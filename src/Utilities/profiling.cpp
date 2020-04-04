#include "profiling.hpp"
#include "Utilities/threadId.hpp"

#include <algorithm>
#include <thread>

namespace ph {

using clock = std::chrono::steady_clock;

//bool ThreadProfilingManager::isValid() const
//{
//	return mThreadId == -1;
//}
//
//void ThreadProfilingManager::setValidId(size_t threadId)
//{
//	mThreadId = static_cast<int>(threadId);
//}

ProfilingResult::id ThreadProfilingManager::commitResultStart(std::string&& name, std::vector<std::pair<std::string, std::string>>&& args)
{
	if (mThreadId == -1)
		mThreadId = ThreadId::getCurrentThreadId();
	auto& result = mResults.emplace_back();
	result.name = std::move(name);
	result.args = std::move(args);
	result.resultId = mNextResultId++;
	result.startTime = std::chrono::time_point_cast<std::chrono::microseconds>(clock::now()).time_since_epoch().count();
	return result.resultId;
}

void ThreadProfilingManager::commitResultEnd(ProfilingResult::id id)
{
	for (auto& result : mResults)
	{
		if (result.resultId == id)
		{
			result.isFinished = true;
			auto endTime = std::chrono::time_point_cast<std::chrono::microseconds>(clock::now()).time_since_epoch().count();
			result.duration = endTime - result.startTime;
			return;
		}
	}

	// PH_UNEXPECTED...
}

bool ThreadProfilingManager::hasCommitedResults() const
{
	return (mResults.empty()) ? false : mResults.front().isFinished;
}

std::vector<ProfilingResult> ThreadProfilingManager::getCommitedResults()
{
	if (hasCommitedResults())
		return std::move(mResults);
	return {};
}

thread_local ThreadProfilingManager MainProfilingManager::mThreadManager;
std::ofstream MainProfilingManager::mOutputFile;
std::mutex MainProfilingManager::mFileMutex;
bool MainProfilingManager::mIsActive = false;

ProfilingResult::id MainProfilingManager::commitResultStart(std::string name, std::vector<std::pair<std::string, std::string>> args)
{
	return mThreadManager.commitResultStart(std::move(name), std::move(args));
}

void MainProfilingManager::commitResultEnd(ProfilingResult::id id)
{
	if (!mIsActive)
	{
		//PH_WARNING...
		return;
	}

	mThreadManager.commitResultEnd(id);
	auto results = mThreadManager.getCommitedResults();
	if (!results.empty())
	{
		std::lock_guard lock(mFileMutex);

		for (const auto& result : results)
		{
			mOutputFile << ",{\"dur\":" << result.duration
						<< ",\"name\":\"" << result.name
						<< "\",\"ph\":\"X\",\"pid\":0,\"tid\":" << ThreadId::getCurrentThreadId()
						<< ",\"ts\":" << result.startTime;

			if (!result.args.empty())
			{
				mOutputFile << ",\"args\":{";
				bool firstPair = true;
				for (const auto& pair : result.args)
				{
					if (!firstPair)
						mOutputFile << ',';
					firstPair = false;
					mOutputFile << "\"" << pair.first
								<< "\":\"" << pair.second
								<< "\"";
				}
				mOutputFile << '}';
			}

			mOutputFile << '}';
		}
		// {"dur":118502,"name":"ph::parseScene","ph":"X","pid":0,"tid":1,"ts":0,"args":{"arg":"value"}}
	}
}

void MainProfilingManager::beginSession(const std::string& filepath)
{
	std::lock_guard lock(mFileMutex);
	mOutputFile.open(filepath);
	mOutputFile << "{\"otherData\": {},\"traceEvents\":[{\"args\":{\"name\":\"PopHead\"},\"cat\":\"__metadata\",\"name\":\"process_name\",\"ph\":\"M\",\"pid\":0}";
	mIsActive = true;
}

void MainProfilingManager::endSession()
{
	std::lock_guard lock(mFileMutex);
	mOutputFile << "]}" << std::flush;
	mOutputFile.close();
	mIsActive = false;
}

ProfilingTimer::ProfilingTimer(const char* name, std::vector<std::pair<std::string, std::string>> args)
{
	resultId = MainProfilingManager::commitResultStart(name, std::move(args));
}

ProfilingTimer::~ProfilingTimer()
{
	MainProfilingManager::commitResultEnd(resultId);
}

}
