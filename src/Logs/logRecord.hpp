#pragma once

#include <string>

namespace ph {

	enum class LogLevel : std::size_t {
		Info,
		Warning,
		Error,
		Critical,
		Count // WARNING: Keep it last
	};

	std::string logLevelToString(LogLevel level);

	struct LogRecord
	{
		LogLevel level;
		std::string message;
		std::string filePath;
		unsigned short fileLine;
		float secondsFromStart;
		std::string time;
	};

}