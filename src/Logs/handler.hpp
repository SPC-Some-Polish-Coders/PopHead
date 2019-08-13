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

		void setPathFilter(const std::string& path, bool allowed);
		void setLogLevelFilter(LogLevel level, bool allowed);

		bool isPathAllowed(const std::string& path) const;
		bool isLogLevelAllowed(LogLevel level) const;

		void enableAllPaths();
		void disableAllPaths();
		void enableAllLogLevels();

	private:
		virtual void utilizeLog(const LogRecord& logRecord) = 0;
		bool isPassedByFilter(const LogRecord& logRecord) const;

		void initializeLogLevels();

	private:
		std::vector<std::string> mAllowedPaths;
		std::vector<std::string> mDisallowedPaths;
		std::vector<std::pair<LogLevel, bool>> mAllowedLogLevels;
	};
}
