#pragma once
#include <string>

class Timer
{
public:
	Timer(float simMultipier = 1.0f);
	virtual ~Timer();

	float			GetTimeFrame() const;
	float			GetTimeSim() const;
	void			SetSimMultiplier(const float simMultiplier);

public:
	virtual bool	Start();
	virtual void 	OnSuspend();
	virtual void 	Update();
	virtual void 	OnResume();
	virtual void 	Stop();

	virtual std::string getName()		{ return "Timer"; }

private:
    typedef long long   TimeUnits;

    TimeUnits nanoTime();

    TimeUnits   m_timeLastFrame;
    float       m_frameDelta;
    float       m_simMultiplier;
    float       m_simDelta;
};

inline float Timer::GetTimeFrame() const
{
	return m_frameDelta;
}

inline float Timer::GetTimeSim() const
{
	return m_simDelta;
}

inline void Timer::SetSimMultiplier(const float simMultiplier)
{
	m_simMultiplier = simMultiplier;
}
