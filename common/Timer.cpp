#include "Timer.h"

#include <time.h>
#include "Logger.hpp"

Timer::Timer(float simMultiplier)
	: m_timeLastCall(0)
	, m_callDelta(0.0f)
    , m_simMultiplier(simMultiplier)
	, m_simDelta(0.0f)
{
}

Timer::~Timer()
{
}

Timer::TimeUnits Timer::nanoTime()
{
	timespec now;
	int err = clock_gettime(CLOCK_MONOTONIC, &now);
	if (err)
	{
	    DLOG() << ARG(err);
	}
	return now.tv_sec*1000000000LL + now.tv_nsec;
}

Timer& Timer::Start()
{
	m_timeLastCall = nanoTime();
    return *this;
}

void Timer::OnSuspend()
{
}

Timer& Timer::Update()
{
	// Get the delta between the last frame and this
	TimeUnits currentTime = nanoTime();
	const float MULTIPLIER = 0.000000001f;
	m_callDelta = MULTIPLIER * (currentTime - m_timeLastCall);
	m_timeLastCall = currentTime;

	m_simDelta = m_callDelta * m_simMultiplier;
    return *this;
}

void Timer::OnResume()
{
	m_timeLastCall = nanoTime();
}

Timer& Timer::Stop()
{
    return *this;
}
