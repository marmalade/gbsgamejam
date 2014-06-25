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
 
#if !defined(_TIMERS_H_)
#define _TIMERS_H_

#include "s3e.h"	// Basic types

/**
 * @class Timer
 *
 * @brief A timer can be used to time the duration of events as well as create delays in-game
 *
 * Below is a short example showing how to use a timer
 * @code
 *    // Create the timer
 *    Timer timer1;
 *
 *    // Elsewhere in our code we start a 2 second timer
 *    timer1.setDuration(2000, true);
 *
 *    // Elsewhere in our code we check to see if the timer has expired
 *    if (timer1.HasTimedOut())
 *    {
 *        // Do something
 8    }
 * @endcode
 
 *
 */

class Timer
{
protected:
    bool        Started;            ///< true if timer started
    uint64 		LastTime;			///< Time that the timer was set (milliseconds)

    // Properties
private:
    uint64		Duration;			///< Duration of timer in milliseconds
    bool		AutoReset;			///< Auto reset the timer when it times out
public:
    void		setDuration(uint64 millseconds_duration, bool start = true);		///< Sets the duration of the timer
    uint64		getDuration() const;												///< Gets the duration of the timer
    void		setAutoReset(bool auto_reset);										///< If true the timer will auto restart once it times out
    bool		getAutoReset() const;												///< Returns auto reset status
    bool		hasStarted() const;													///< Returns true if the timer has started
    bool		hasStopped() const;													///< Returns true if the timer has stopped
    // Properties end

public:
    Timer() : LastTime(0), Duration(0), AutoReset(false), Started(false) { }
    Timer(int millseconds_duration);
    virtual ~Timer() {	}
    
    virtual bool HasTimedOut();														///< Returns true if the timer has timed out
    uint64		GetElapsedTime() const;												///< Return the amount of time since the timer was started
    uint64		GetTimeDiff(uint64 this_time) const;								///< returns the difference in time between the supplied time and the timers current time
    uint64		GetTimeDiff() const;												///< returns the difference in time between the current time and the timers current time
    uint64		GetTimeLeft() const;												///< Returns the amount of time left before the timer times out
    void		Reset();															///< Restarts the timer
    void		Start();															///< Starts the timer
    void		Stop();																///< Stops the timer
};

#endif	// _TIMER_
