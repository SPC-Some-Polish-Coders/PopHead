#pragma once

#include "Utilities/debug.hpp"

#include "Logs/logger.hpp"
#include "Logs/logRecord.hpp"
#include "Logs/criticalError.hpp"

#define PH_LOG(logLevel, message)\
	ph::Logger::createLog(logLevel, message, __FILE__, static_cast<unsigned short>(__LINE__))

#define PH_ASSERT_EXPRESSION(expression, code)\
	if(!(expression)) code


#ifdef PH_DEBUG_LOGS_ENABLED

#define PH_LOG_INFO(message)\
	PH_LOG(ph::LogLevel::Info, message)

#define PH_LOG_WARNING(message)\
	PH_LOG(ph::LogLevel::Warning, message)

#define PH_ASSERT_WARNING(expression, message)\
	PH_ASSERT_EXPRESSION(expression, PH_LOG_WARNING(message))

#else

#define PH_LOG_INFO(message)
#define PH_LOG_WARNING(message)
#define PH_ASSERT_WARNING(expression, message)

#endif // PH_DEBUG_LOGS_ENABLED


#define PH_LOG_ERROR(message)\
	PH_LOG(ph::LogLevel::Error, message)
#define PH_LOG_CRITICAL(message)\
	{ PH_LOG(ph::LogLevel::Critical, message); throw ph::CriticalError(message); }

#define PH_ASSERT_ERROR(expression, message)\
	PH_ASSERT_EXPRESSION(expression, PH_LOG_ERROR(message))
#define PH_ASSERT_CRITICAL(expression, message)\
	PH_ASSERT_EXPRESSION(expression, PH_LOG_CRITICAL(message))


#ifdef PH_DEBUG_LOGS_ENABLED

#define PH_UNEXPECTED_SITUATION(message)\
	PH_LOG_CRITICAL(message)
#define PH_ASSERT_UNEXPECTED_SITUATION(expression, message)\
	PH_ASSERT_EXPRESSION(expression, PH_UNEXPECTED_SITUATION(message))

#else

#define PH_UNEXPECTED_SITUATION(message)
#define PH_ASSERT_UNEXPECTED_SITUATION(expression, message)

#endif // PH_DEBUG_LOGS_ENABLED
