namespace ph {

#define Kilobytes(n) n * 1024
#define Megabytes(n) Kilobytes(n * 1024)

void* allocateArena(size_t arenaSize);
void deallocateArena(void* ptrToArenaStart);

}
