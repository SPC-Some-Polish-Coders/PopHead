#include "profiling.hpp"
#include "Utilities/threadId.hpp"
#include <imgui.h>
#include <algorithm>
#include <thread>

extern bool debugWindowOpen;

namespace ph {

using clock = std::chrono::steady_clock;

ProfilingResult::id MainProfilingManager::ThreadProfilingManager::commitResultStart(std::string&& name, std::vector<std::pair<std::string, std::string>>&& args)
{
	auto& result = mResults.emplace_back();
	result.name = std::move(name);
	result.args = std::move(args);
	result.resultId = mNextResultId++;
	result.startTime = std::chrono::time_point_cast<std::chrono::microseconds>(clock::now()).time_since_epoch().count();
	return result.resultId;
}

void MainProfilingManager::ThreadProfilingManager::commitResultEnd(ProfilingResult::id id)
{
	for (auto& result : mResults)
	{
		if (result.resultId == id)
		{
			auto endTime = std::chrono::time_point_cast<std::chrono::microseconds>(clock::now()).time_since_epoch().count();
			result.duration = static_cast<unsigned int>(endTime - result.startTime);
			if (mResults[0].resultId == id)
				mIsFirstResultFinished = true;
			return;
		}
	}
}

bool MainProfilingManager::ThreadProfilingManager::hasCommitedResults() const
{
	return mIsFirstResultFinished;
}

std::vector<ProfilingResult> MainProfilingManager::ThreadProfilingManager::getCommitedResults()
{
	if (hasCommitedResults())
	{
		mIsFirstResultFinished = false;
		return std::move(mResults);
	}
	return {};
}

thread_local MainProfilingManager::ThreadProfilingManager MainProfilingManager::mThreadManager;
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
		return;

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

ImGuiProfilingTimer::ImGuiProfilingTimer(const char* name)
{
	mResult.name = name;
	clock.restart();
}

ImGuiProfilingTimer::~ImGuiProfilingTimer()
{
	mResult.duration = clock.getElapsedTime().asSeconds();
	ImGuiProfiling::commitResult(mResult);
}

namespace ImGuiProfiling {

	static std::vector<ImGuiProfilingResult> results;
	static float timeFromLastDisplay;
	static float displayDelay = 0.01f;
	static bool clear; 
	static bool pause;

	void commitResult(const ImGuiProfilingResult& result)
	{
		if(timeFromLastDisplay > displayDelay)
		{
			if(clear)
			{
				results.clear();
				clear = false;
			}

			for(auto& res : results)
			{
				if(res.name == result.name)
				{
					res.duration += result.duration;
					return;
				}
			}
			
			results.emplace_back(result);
		}
	}

	void flush(float dt)
	{
		if(debugWindowOpen && ImGui::BeginTabItem("profiling"))
		{
			ImGui::SliderFloat("display delay", &displayDelay, 0.01f, 2.f);
			ImGui::Checkbox("pause display", &pause);
			ImGui::Separator();
			
			if(timeFromLastDisplay > displayDelay)
			{
				std::sort(results.begin(), results.end(), 
				[](const ImGuiProfilingResult& a, const ImGuiProfilingResult& b)
				{
					return a.duration > b.duration;
				});
			}

			ImGui::Columns(3, nullptr);
			ImGui::BulletText("Time of all calls in seconds");
			ImGui::NextColumn();
			ImGui::BulletText("Percent of time of all calls relative to 60Hz");
			ImGui::NextColumn();
			ImGui::BulletText("Function name");
			ImGui::NextColumn();

			for(auto& res : results)
			{	
				ImGui::Text("%f", res.duration);
				ImGui::NextColumn();

				ImGui::Text("%f", res.duration * 6000.f);
				ImGui::NextColumn();

				ImGui::Text(res.name.c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);

			ImGui::EndTabItem();
		}

		if(timeFromLastDisplay > displayDelay)
		{
			clear = true;
			timeFromLastDisplay = 0.f;
		}

		if(!pause)
		{
			timeFromLastDisplay += dt;
		}
	}
}

}
