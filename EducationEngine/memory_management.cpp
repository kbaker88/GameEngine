#include "memory_management.h"

struct MemoryNode
{
	MemoryNode* NextNode;
	void* Data;
};

uint8 Memory_Init()
{
	return 0;
}

uint32 Memory_Add(void* Data)
{
	return 0;
}

uint8 Memory_Delete(uint32 DataID)
{
	return 0;
}

void* Memory_Search(uint32 DataID)
{
	return NULL;
}