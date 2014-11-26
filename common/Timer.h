#pragma once
#include <string>

class Timer
{
public:
	Timer(float simMultipier = 1.0f);
	virtual ~Timer();

	float			getDelta() const;
	float			getSimDelta() const;
	Timer&			setSimMultiplier(const float simMultiplier);

public:
	virtual Timer&	Start();
	virtual void 	OnSuspend();
	virtual Timer& 	Update();
	virtual void 	OnResume();
	virtual Timer& 	Stop();

	virtual std::string getName()		{ return "Timer"; }

private:
    typedef long long   TimeUnits;

    TimeUnits nanoTime();

    TimeUnits   m_timeLastCall;
    float       m_callDelta;
    float       m_simMultiplier;
    float       m_simDelta;
};

inline float Timer::getDelta() const
{
	return m_callDelta;
}

inline float Timer::getSimDelta() const
{
	return m_simDelta;
}

inline Timer& Timer::setSimMultiplier(const float simMultiplier)
{
	m_simMultiplier = simMultiplier;
	return *this;
}
