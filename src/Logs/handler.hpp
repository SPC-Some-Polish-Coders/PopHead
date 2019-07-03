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
		void setTypeAllowing(LogType type, bool allowed);

		bool isModuleAllowed(const std::string& moduleName) const;
		bool isTypeAllowed(LogType type) const;

	private:
		virtual void utilizeLog(const LogRecord& logRecord) = 0;

		bool isPassedByFilter(const LogRecord& logRecord) const;

		std::vector<std::pair<std::string, bool>> mAllowedModules;
		std::vector<std::pair<LogType, bool>> mAllowedTypes;
	};
}
