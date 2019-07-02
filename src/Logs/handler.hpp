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

	private:
		virtual void utilizeLog(const LogRecord& logRecord) = 0;

		bool isPassedByFilter(const LogRecord& logRecord);

		std::vector<std::pair<std::string, bool>> allowedModules;
		std::vector<std::pair<LogType, bool>> allowedTypes;
	};
}
