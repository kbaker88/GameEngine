#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "utility.h"

inline void* Memory_Itr();

void Memory_Initialize(void* MemoryBuffer, unsigned int Size);
char* Memory_AllocCharArr(unsigned int Size);
void Memory_Clean();

#endif