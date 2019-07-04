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

		mAllowedTypes = {
			{LogType::Info, false},
			{LogType::Error, false},
			{LogType::FromUser, false},
			{LogType::Warning, false},
			{LogType::Exception, false},
			{LogType::UnhandledException, false}
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

	void Handler::setTypeAllowing(LogType type, bool allowed)
	{
		auto iter = std::find_if(mAllowedTypes.begin(), mAllowedTypes.end(), [&type](const std::pair<LogType, bool>& pair) {
			return pair.first == type;
			});

		if (iter != mAllowedTypes.end())
			iter->second = allowed;
	}

	bool Handler::isModuleAllowed(const std::string& moduleName) const
	{
		auto iter = std::find_if(mAllowedModules.begin(), mAllowedModules.end(), [&moduleName](const std::pair<std::string, bool>& pair) {
			return pair.first == moduleName;
			});

		return iter != mAllowedModules.end() && iter->second;
	}

	bool Handler::isTypeAllowed(LogType type) const
	{
		auto iter = std::find_if(mAllowedTypes.begin(), mAllowedTypes.end(), [&type](const std::pair<LogType, bool>& pair) {
			return pair.first == type;
			});

		return iter != mAllowedTypes.end() && iter->second;
	}

	bool Handler::isPassedByFilter(const LogRecord& logRecord) const
	{
		return isModuleAllowed(logRecord.moduleName) && isTypeAllowed(logRecord.type);
	}
}
