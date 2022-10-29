
#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed()
{
	if (isRunningCond)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::Restart()
{
	isRunningCond = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!isRunningCond)
		return false;
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		isRunningCond = false;
		return true;
	}
}

bool Timer::Start()
{
	if (isRunningCond)
	{
		return false;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isRunningCond = true;
		return true;
	}
}