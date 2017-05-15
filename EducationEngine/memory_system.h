#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "utility.h"

// NOTE: This system is not fully implemented yet
// TODO: Remove all news and mallocs and use this system when complete.
inline void* Memory_Itr();

void Memory_Initialize(void* MemoryBuffer, unsigned int Size);
char* Memory_AllocCharArr(unsigned int Size);
void Memory_Clean();

#endif