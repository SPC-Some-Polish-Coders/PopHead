#pragma once

#include <stdexcept>

#define PH_BREAKPOINT()

#ifndef PH_DISTRIBUTION
#ifndef PH_TESTS
#ifdef _MSC_VER

#undef PH_BREAKPOINT
#define PH_BREAKPOINT() __debugbreak()

#endif // _MSC_VER
#endif // !PH_TESTS
#endif // !PH_DISTRIBUTION


#ifndef PH_DISTRIBUTION

#define PH_DEBUG_LOGS_ENABLED

#endif // !PH_DISTRIBUTION


#ifdef PH_DISTRIBUTION

#define PH_ASSERT(expression, message) ((void)0)
#define PH_EXCEPTION(message) throw std::runtime_error(message)
#define PH_ASSERT_EXCEPTION(expression, message) ((void)0)

#else // !PH_DISTRIBUTION

#define PH_ASSERT(expression, message) if (!(expression)){ PH_LOG(ph::LogLevel::Error, message); PH_BREAKPOINT();}
#define PH_EXCEPTION(message) {PH_LOG(ph::LogLevel::Critical, message); PH_BREAKPOINT(); throw std::runtime_error(message);}
#define PH_ASSERT_EXCEPTION(expression, message) if(!(expression)) PH_EXCEPTION(message)

#endif // !PH_DISTRIBUTION
