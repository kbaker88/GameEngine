#include "clock.h"

Clock GlobalClock;

float Clock_GetCurrentTime()
{
	int64 CounterElapsed = Platform_GetCPUCounter() - GlobalClock.StartCPUCount;
	float SecondsElapsed = ((float)CounterElapsed /
		(float)GlobalClock.Frequency);
	return SecondsElapsed;
}

Clock::Clock()
{
	Frequency = Platform_GetTimeFrequency(); // TODO: This is only to do
										     // done once at the start.
										     // remove this when better organized
	StartCPUCount = Platform_GetCPUCounter();
}

Timer::Timer()
{
	StartTime = Clock_GetCurrentTime();
	TimerState = 0;
}

void Timer::Start()
{
	StartTime = Clock_GetCurrentTime();
	TimerState = 1;
}

void Timer::Stop()
{
	if (TimerState == 1)
	{
		EndTime = Clock_GetCurrentTime() - StartTime;
	}
}

float Timer::GetTime()
{
	if (TimerState == 1)
	{
		return (Clock_GetCurrentTime() - StartTime);
	}
	else
	{
		return EndTime;
	}
}
