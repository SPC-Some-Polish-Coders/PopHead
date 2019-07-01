#include "logRecord.hpp"

std::string ph::logTypeToString(LogType type)
{
	switch (type)
	{
	case ph::LogType::Info:
		return "INFO";
	case ph::LogType::Error:
		return "Error";
	case ph::LogType::Warning:
		return "Warning";
	case ph::LogType::FromUser:
		return "FromUser";
	case ph::LogType::Exception:
		return "Exception";
	case ph::LogType::UnhandledException:
		return "UnhandledException";
	}

	// TODO: add exception throwing
	return "";
}
