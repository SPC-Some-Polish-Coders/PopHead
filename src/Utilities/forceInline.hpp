#pragma once

#define FORCE_INLINE_ENABLED 1

#if FORCE_INLINE_ENABLED
	#ifdef _MSC_VER
		#define FORCE_INLINE __forceinline
	#else
		#define FORCE_INLINE
	#endif
#else
	#define FORCE_INLINE
#endif
