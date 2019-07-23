#pragma once

#include "Utilities/debug.hpp"

#include "Logs/logger.hpp"
#include "Logs/logRecord.hpp"

#define PH_LOG(logType, message)\
	ph::Logger::createLog(logType, message, __FILE__, static_cast<unsigned short>(__LINE__))

// temporary macro
#define INFO_LOGS_ENABLED

#ifdef INFO_LOGS_ENABLED

#define PH_LOG_INFO(message)\
	PH_LOG(ph::LogLevel::Info, message)

#else

#define PH_LOG_INFO(message)

#endif // INFO_LOGS_ENABLED
