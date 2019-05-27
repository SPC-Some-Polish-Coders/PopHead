#ifndef POPHEAD_UTILITIES_DEBUG_H_
#define POPHEAD_UTILITIES_DEBUG_H_

#include "Logs/logger.hpp"

#ifdef PH_RELEASE
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
	PopHead::Logs::Logger::getLogger().writeLog(PopHead::Logs::LogData{message, PopHead::Utilities::Parser::toModuleName(std::string(__FILE__)), logType})

#define PH_ASSERT(expression, message) (void)((expression) || (PH_LOG(PopHead::Logs::LogType::Error, message), PH_BREAKPOINT(), 0))

#define PH_EXCEPTION(message) (void)(PH_LOG(PopHead::Logs::LogType::Error, message), PH_BREAKPOINT(), throw std::runtime_error(message), 0)
#endif // !PH_RELEASE

#endif // !POPHEAD_UTILITIES_DEBUG_H_
