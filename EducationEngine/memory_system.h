#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "platform_layer.h"

// NOTE: Temporary until memory system is complete.
#define MEMORY_ON 0

// NOTE: This system is not fully implemented yet
// TODO: Remove all news and mallocs and use this system when complete.
#if MEMORY_ON

void 
Memory_Initialize(uint64 Size);

char* 
Memory_Allocate(char* Ptr, unsigned int Size);
uint8*
Memory_Allocate(uint8* Ptr, unsigned int Size);
uint16*
Memory_Allocate(uint16* Ptr, unsigned int Size);
int32*
Memory_Allocate(int32* Ptr, unsigned int Size);
uint32*
Memory_Allocate(uint32* Ptr, unsigned int Size);
float*
Memory_Allocate(float* Ptr, unsigned int Size);
float**
Memory_Allocate(float** Ptr, unsigned int Size);
void*
Memory_GetMemPtr();
void
Memory_AdvanceItr(unsigned int Size);
void
Memory_Clean();

#endif

#endif