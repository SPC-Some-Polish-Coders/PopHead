#pragma once

#include <string>

namespace ph {

	enum class LogType : std::size_t {
		Info,
		Error,
		Warning,
		FromUser,
		Exception,
		UnhandledException,
		Count // WARNING: Keep it last
	};

	std::string logTypeToString(LogType type);

	struct LogRecord
	{
		LogType type;
		std::string message;
		std::string fileName;
		std::string moduleName;
		unsigned short fileLine;
		unsigned int secondsFromStart;
		std::string time;
	};

}