#include "memory_system.h"

// TODO: Temporary vars
void* MemoryStart;
void* MemoryItr;
void* MemoryEnd;

void
Memory_Initialize(void* MemoryBuffer, unsigned int Size)
{
	MemoryStart = MemoryBuffer;
	MemoryItr = MemoryStart;
	MemoryEnd = (char*)MemoryStart + Size;
}

void*
Memory_Itr()
{
	return MemoryItr;
}

char*
Memory_AllocCharArr(unsigned int Size)
{
	char *Start = (char*)MemoryItr;
	char *End = Start + Size;
	*End = '\0';
	MemoryItr = End + 1;
	return Start;
}

void
Memory_Clean()
{

}