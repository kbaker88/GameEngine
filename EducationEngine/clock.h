#ifndef CLOCK_H
#define CLOCK_H

#include "platform_layer.h"
// TODO: Limit all functions here to one call to platform per frame. meaning
// global time keeping and using the global to calculate all others.

struct Clock
{
	Clock();

	int64 Frequency, StartCPUCount, CurrentCPUCount;
};

float Clock_GetCurrentTime();

struct Timer
{
	Timer();

	void Start();
	void Stop();
	float GetTime();

	float StartTime, EndTime;
	int8 TimerState;
};

#endif
