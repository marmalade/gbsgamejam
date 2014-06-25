/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
 
#include "Timers.h"

uint64 Timer::getDuration() const				{ return Duration; }
void Timer::setAutoReset(bool auto_reset)		{ AutoReset = auto_reset; }
bool Timer::getAutoReset() const				{ return AutoReset; }
bool Timer::hasStarted() const					{ return Started; }
bool Timer::hasStopped() const                  { return !Started; }

void Timer::setDuration(uint64 millseconds_duration, bool start)
{
	Duration = millseconds_duration;
	if (start)
		Start();
}

Timer::Timer(int millseconds_duration)
{
	LastTime = 0;
	Duration = 0;
	AutoReset = false;
	Started = false;
	setDuration(millseconds_duration);
}

bool Timer::HasTimedOut()
{
	if (!Started)
		return false;

	if (Duration == 0)	// Timer of 0 duration never times out
	{
		return false;
	}
		
	uint64 time_diff = GetElapsedTime();
	
	if (time_diff > Duration)
	{
		if (AutoReset)
		{
			LastTime = s3eTimerGetMs();
		}
		else
			Started = false;
		return true;
	}
	
	return false;
}

uint64 Timer::GetElapsedTime() const
{
	return s3eTimerGetMs() - LastTime;
}

uint64 Timer::GetTimeDiff(uint64 this_time) const
{
	return this_time - LastTime;
}

uint64 Timer::GetTimeDiff() const
{
	return s3eTimerGetMs() - LastTime;
}

uint64 Timer::GetTimeLeft() const
{
	return Duration - (s3eTimerGetMs() - LastTime);
}

void Timer::Reset()
{
	setDuration(Duration);
}

void Timer::Start()
{
	LastTime = s3eTimerGetMs();
	Started = true;
}

void Timer::Stop()
{
	Started = false;
}

