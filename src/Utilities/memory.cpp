#ifdef PH_WINDOWS

#include <windows.h>

namespace ph {

void* allocateArena(size_t arenaSize)
{
	return VirtualAlloc(nullptr, arenaSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void deallocateArena(void* ptrToArenaStart)
{
	VirtualFree(ptrToArenaStart, 0, MEM_RELEASE);
}

}

#else
	#error You have to implement memory allocation functions for your platform
#endif
