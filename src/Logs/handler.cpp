#include "handler.hpp"

#include <algorithm>

namespace ph {
	
	Handler::Handler()
	{
		allowedModules = {
			{"Audio", true},
			{"EfficiencyRegister", true},
			{"Gui", true},
			{"Input", true},
			{"Logs", true},
			{"Physics", true},
			{"Renderer", true},
			{"Resources", true},
			{"States", true},
			{"Terminal", true},
			{"Utilities", true},
			{"World", true}
		};

		allowedTypes = {
			{LogType::Info, true},
			{LogType::Error, true},
			{LogType::FromUser, true},
			{LogType::Warning, true},
			{LogType::Exception, true},
			{LogType::UnhandledException, true}
		};
	}

	void Handler::handleLog(const LogRecord& logRecord)
	{
		if (isPassedByFilter(logRecord))
			utilizeLog(logRecord);
	}

	void Handler::setModuleAllowing(const std::string& moduleName, bool allowed)
	{
		auto iter = std::find_if(allowedModules.begin(), allowedModules.end(), [&moduleName](const std::pair<std::string, bool>& pair) {
			return pair.first == moduleName;
			});

		if (iter != allowedModules.end())
			iter->second = allowed;
	}

	void Handler::setTypeAllowing(LogType type, bool allowed)
	{
		auto iter = std::find_if(allowedTypes.begin(), allowedTypes.end(), [&type](const std::pair<LogType, bool>& pair) {
			return pair.first == type;
			});

		if (iter != allowedTypes.end())
			iter->second = allowed;
	}

	bool Handler::isModuleAllowed(const std::string& moduleName) const
	{
		auto iter = std::find_if(allowedModules.begin(), allowedModules.end(), [&moduleName](const std::pair<std::string, bool>& pair) {
			return pair.first == moduleName;
			});

		return iter != allowedModules.end() && iter->second;
	}

	bool Handler::isTypeAllowed(LogType type) const
	{
		auto iter = std::find_if(allowedTypes.begin(), allowedTypes.end(), [&type](const std::pair<LogType, bool>& pair) {
			return pair.first == type;
			});

		return iter != allowedTypes.end() && iter->second;
	}

	bool Handler::isPassedByFilter(const LogRecord& logRecord) const
	{
		return isModuleAllowed(logRecord.moduleName) && isTypeAllowed(logRecord.type);
	}
}
