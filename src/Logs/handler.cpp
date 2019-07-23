#include "handler.hpp"

#include <algorithm>

namespace ph {
	
	Handler::Handler()
	{
		mAllowedModules = {
			{"Audio", false},
			{"EfficiencyRegister", false},
			{"Gui", false},
			{"Input", false},
			{"Logs", false},
			{"Physics", false},
			{"Renderer", false},
			{"Resources", false},
			{"States", false},
			{"Terminal", false},
			{"Utilities", false},
			{"World", false}
		};

		mAllowedLogLevels = {
			{LogLevel::Info, false},
			{LogLevel::Error, false},
			{LogLevel::Warning, false},
			{LogLevel::Critical, false}
		};
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
}
