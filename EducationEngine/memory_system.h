#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "utility.h"

// NOTE: This system is not fully implemented yet
// TODO: Remove all news and mallocs and use this system when complete.
#if MEMORY_ON

void 
Memory_Initialize(void* MemoryBuffer, unsigned int Size);

char* 
Memory_Allocate(char* Ptr, unsigned int Size);
void
Memory_Clean();

#endif

#endif