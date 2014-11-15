#include "Timer.h"

#include <time.h>
#include "Logger.hpp"

Timer::Timer(float simMultiplier)
	: m_timeLastFrame(0)
	, m_frameDelta(0.0f)
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

bool Timer::Start()
{
	m_timeLastFrame = nanoTime();
	return true;
}

void Timer::OnSuspend()
{
}

void Timer::Update()
{
	// Get the delta between the last frame and this
	TimeUnits currentTime = nanoTime();
	const float MULTIPLIER = 0.000000001f;
	m_frameDelta = MULTIPLIER * (currentTime - m_timeLastFrame);
	m_timeLastFrame = currentTime;

	m_simDelta = m_frameDelta * m_simMultiplier;
}

void Timer::OnResume()
{
	m_timeLastFrame = nanoTime();
}

void Timer::Stop()
{
}
