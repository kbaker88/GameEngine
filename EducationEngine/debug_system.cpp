#include "debug_system.h"

int64 TimeFrequency, LastCounter;
float FPS;

void DEBUG_Initialize()
{
	TimeFrequency = Platform_GetTimeFrequency();
	LastCounter = Platform_GetCPUCounter();
}

float DEBUG_GetFPS()
{
	FPS = (float)TimeFrequency /
		(float)(Platform_GetCPUCounter() - LastCounter);

	LastCounter = Platform_GetCPUCounter();
	return FPS;
}