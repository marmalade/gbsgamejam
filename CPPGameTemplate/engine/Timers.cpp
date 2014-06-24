// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

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

