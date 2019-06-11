#ifndef POPHEAD_UTILITIES_DEBUG_H_
#define POPHEAD_UTILITIES_DEBUG_H_

#include "Logs/logger.hpp"

#ifdef PH_DISTRIBUTION
#define PH_BREAKPOINT() ((void)0)

#define PH_LOG(logType, message) ((void)0)

#define PH_ASSERT(expression, message) ((void)0)

#define PH_EXCEPTION(message) throw std::runtime_error(message)
#else
#ifdef _MSC_VER
#define PH_BREAKPOINT() __debugbreak()
#else
#define PH_BREAKPOINT() ((void)0)
#endif // !_MSC_VER

#define PH_LOG(logType, message)\
	ph::Logger::getLogger().writeLog(ph::LogData{message, ph::Path::toModuleName(std::string(__FILE__)), logType})

#define PH_ASSERT(expression, message) (void)((expression) || (PH_LOG(ph::LogType::Error, message), PH_BREAKPOINT(), 0))

#define PH_EXCEPTION(message) (void)(PH_LOG(ph::LogType::Error, message), PH_BREAKPOINT(), throw std::runtime_error(message), 0)
#endif // !PH_RELEASE

#endif // !POPHEAD_UTILITIES_DEBUG_H_
