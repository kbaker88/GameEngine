#include "memory_system.h"

// TODO: Temporary vars
void* MemoryStore;
void* MemoryItr;
void* MemoryEnd;
unsigned long long MemoryStoreSize;


void
Memory_Initialize(void* MemoryBuffer, unsigned int Size)
{
	MemoryStoreSize = Gigabytes(2);
	MemoryStore = Platform_AllocateMemory(&MemoryStoreSize);

	MemoryItr = MemoryStore;

	MemoryEnd = (char*)MemoryStore + Size;
}

char*
Memory_Allocate(char* Ptr, unsigned int Size)
{
	Ptr = (char*)MemoryItr;
	MemoryItr = (char*)MemoryItr + Size;
	char *End = (char*)MemoryItr - 1;
	*End = '\0';
	return Ptr;
}

void
Memory_Clean()
{
	Platform_DeallocateMemory(MemoryStore, &MemoryStoreSize);
}