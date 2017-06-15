#ifndef CLOCK_H
#define CLOCK_H

#include "utility.h"

// TODO: Limit all functions here to one call to platform per frame. meaning
// global time keeping and using the global to calculate all others.

#if 0
struct Clock
{
	int64 Frequency, StartCPUCount, CurrentCPUCount;
};

struct Timer
{
	float StartTime, EndTime;
	int8 TimerState;
};

#else
struct Clock
{
	Clock();

	int64 Frequency, StartCPUCount, CurrentCPUCount;
};

float 
Clock_GetCurrentTime();

struct Timer
{
	Timer();

	void Start();
	void Stop();
	float GetTime();

	float StartTime, EndTime;
	int8 TimerState;
};
#endif // 0

#endif
