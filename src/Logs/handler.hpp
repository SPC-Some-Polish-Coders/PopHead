#pragma once

#include "logRecord.hpp"

#include <string>
#include <vector>
#include <utility>

namespace ph {
	
	class Handler
	{
	public:
		Handler();

		void handleLog(const LogRecord& logRecord);

		void setModuleAllowing(const std::string& moduleName, bool allowed);
		void setLogLevelAllowing(LogLevel level, bool allowed);

		bool isModuleAllowed(const std::string& moduleName) const;
		bool isLogLevelAllowed(LogLevel level) const;

		void enableAllModules();
		void enableAllLogLevels();

	private:
		virtual void utilizeLog(const LogRecord& logRecord) = 0;

		bool isPassedByFilter(const LogRecord& logRecord) const;

		std::vector<std::pair<std::string, bool>> mAllowedModules;
		std::vector<std::pair<LogLevel, bool>> mAllowedLogLevels;
	};
}
