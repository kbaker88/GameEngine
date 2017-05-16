#include "memory_system.h"

#if MEMORY_ON

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

uint8*
Memory_Allocate(uint8* Ptr, unsigned int Size)
{
	Ptr = (uint8*)MemoryItr;
	MemoryItr = (uint8*)MemoryItr + Size;
	return Ptr;
}

uint16*
Memory_Allocate(uint16* Ptr, unsigned int Size)
{
	Ptr = (uint16*)MemoryItr;
	MemoryItr = (uint16*)MemoryItr + Size;
	return Ptr;
}

int32*
Memory_Allocate(int32* Ptr, unsigned int Size)
{
	Ptr = (int32*)MemoryItr;
	MemoryItr = (int32*)MemoryItr + Size;
	return Ptr;
}

uint32*
Memory_Allocate(uint32* Ptr, unsigned int Size)
{
	Ptr = (uint32*)MemoryItr;
	MemoryItr = (uint32*)MemoryItr + Size;
	return Ptr;
}

float*
Memory_Allocate(float* Ptr, unsigned int Size)
{
	Ptr = (float*)MemoryItr;
	MemoryItr = (float*)MemoryItr + Size;
	return Ptr;
}

float**
Memory_Allocate(float** Ptr, unsigned int Size)
{
	Ptr = (float**)MemoryItr;
	MemoryItr = (float**)MemoryItr + Size;
	return Ptr;
}

void*
Memory_GetMemPtr()
{
	return MemoryItr;
}
void
Memory_AdvanceItr(unsigned int Size)
{
	MemoryItr = (char*)MemoryItr + Size;
}

void
Memory_Clean()
{
	Platform_DeallocateMemory(MemoryStore, &MemoryStoreSize);
}
#endif