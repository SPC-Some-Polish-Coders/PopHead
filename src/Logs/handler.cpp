#include "handler.hpp"

#include <algorithm>
#include <filesystem>

namespace ph {
	
	Handler::Handler()
	{
		initializeModules();
		initializeLogLevels();
	}

	void Handler::handleLog(const LogRecord& logRecord)
	{
		if (isPassedByFilter(logRecord))
			utilizeLog(logRecord);
	}

	void Handler::setModuleAllowing(const std::string& moduleName, bool allowed)
	{
		auto iter = std::find_if(mAllowedModules.begin(), mAllowedModules.end(), [&moduleName](const std::pair<std::string, bool>& pair) {
			return pair.first == moduleName;
			});

		if (iter != mAllowedModules.end())
			iter->second = allowed;
	}

	void Handler::setLogLevelAllowing(LogLevel level, bool allowed)
	{
		auto iter = std::find_if(mAllowedLogLevels.begin(), mAllowedLogLevels.end(), [&level](const std::pair<LogLevel, bool>& pair) {
			return pair.first == level;
			});

		if (iter != mAllowedLogLevels.end())
			iter->second = allowed;
	}

	bool Handler::isModuleAllowed(const std::string& moduleName) const
	{
		auto iter = std::find_if(mAllowedModules.begin(), mAllowedModules.end(), [&moduleName](const std::pair<std::string, bool>& pair) {
			return pair.first == moduleName;
			});

		return iter != mAllowedModules.end() && iter->second;
	}

	bool Handler::isLogLevelAllowed(LogLevel level) const
	{
		auto iter = std::find_if(mAllowedLogLevels.begin(), mAllowedLogLevels.end(), [&level](const std::pair<LogLevel, bool>& pair) {
			return pair.first == level;
			});

		return iter != mAllowedLogLevels.end() && iter->second;
	}

	void Handler::enableAllModules()
	{
		for (auto& module : mAllowedModules)
			module.second = true;
	}

	void Handler::enableAllLogLevels()
	{
		for (auto& level : mAllowedLogLevels)
			level.second = true;
	}

	bool Handler::isPassedByFilter(const LogRecord& logRecord) const
	{
		return isModuleAllowed(logRecord.moduleName) && isLogLevelAllowed(logRecord.level);
	}
	void Handler::initializeModules()
	{
		// in Distribution it should be replaced by static list of modules

		std::string module;
		for (auto& p : std::filesystem::directory_iterator("src"))
			if (p.is_directory())
			{
				module = p.path().string();
				module.erase(0, 4);
				mAllowedModules.emplace_back(module, false);
			}

		mAllowedModules.emplace_back("MainDirectory", false);
		mAllowedModules.emplace_back("Tests", false);
	}
	void Handler::initializeLogLevels()
	{
		auto levelsCount = static_cast<std::size_t>(LogLevel::Count);
		mAllowedLogLevels.resize(levelsCount);

		for (std::size_t level = 0; level < levelsCount; ++level)
			mAllowedLogLevels.at(level) = std::make_pair(static_cast<LogLevel>(level), false);
	}
}
