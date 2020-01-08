#pragma once

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

