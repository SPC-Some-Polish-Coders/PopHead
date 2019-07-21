#pragma once

#include "Logs/logs.hpp"

#include <stdexcept>

#define PH_BREAKPOINT() ((void)0)

#ifndef PH_DISTRIBUTION
#ifndef PH_TESTS
#ifdef _MSC_VER

#undef PH_BREAKPOINT
#define PH_BREAKPOINT() __debugbreak()

#endif // _MSC_VER
#endif // !PH_TESTS
#endif // !PH_DISTRIBUTION


#ifdef PH_DISTRIBUTION

#define PH_ASSERT(expression, message) ((void)0)

#define PH_EXCEPTION(message) throw std::runtime_error(message)

#define PH_ASSERT_EXCEPTION(expression, message) ((void)0)

#else // !PH_DISTRIBUTION

#define PH_ASSERT(expression, message) (void)((expression) || (PH_LOG(ph::LogType::Error, message), PH_BREAKPOINT(), 0))

#define PH_EXCEPTION(message) (void)(PH_LOG(ph::LogType::Exception, message), PH_BREAKPOINT(), throw std::runtime_error(message), 0)

#define PH_ASSERT_EXCEPTION(expression, message) (void)((expression) || (PH_EXCEPTION(message), 0))

#endif // !PH_DISTRIBUTION
