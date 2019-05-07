#ifndef POPHEAD_LOGS_ASSERT_H_
#define POPHEAD_LOGS_ASSERT_H_

#include "Logs/logger.hpp"

#ifdef _MSC_VER
#define PH_BREAKPOINT() __debugbreak()
#else
#define PH_BREAKPOINT() ((void)0)
#endif // _MSC_VER

#ifdef PH_RELEASE
#define PH_ASSERT(expression, message) ((void)0)
#else
#define PH_ASSERT(expression, message) (void)((expression) || (PH_LOG(PopHead::Logs::LogType::Error, message), PH_BREAKPOINT(), 0))
#endif // PH_RELEASE

#endif // !POPHEAD_LOGS_ASSERT_H_