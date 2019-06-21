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

struct LogData
{
	std::string message;
	std::string moduleName;
	LogType type;
};

}
