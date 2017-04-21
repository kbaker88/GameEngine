#include "memory_system.h"

void* MemoryStart;
void* MemoryItr;
void* MemoryEnd;

inline void* Memory_Itr()
{
	return MemoryItr;
}