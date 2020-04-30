#include "pch.hpp"
#include "logRecord.hpp"

std::string ph::logLevelToString(LogLevel level)
{
	switch (level)
	{
	case ph::LogLevel::Info:
		return "Info";
	case ph::LogLevel::Error:
		return "Error";
	case ph::LogLevel::Warning:
		return "Warning";
	case ph::LogLevel::Critical:
		return "Critical";
	}

	// TODO: add exception throwing
	return "";
}
