// CClock implementation file (CClock.cpp)

/* Private members:
					unsigned int ClockTime	*/

#include <windows.h>
#include "CClock.h"

CClock::CClock()
{
	mulClockTime = 0;
	mulPauseTime = 0;
}

void CClock::Start()
{
	mulClockTime = GetTime();
}

void CClock::Wait(unsigned long ulWaitTime)
{
	while ((GetTime() - mulClockTime) < ulWaitTime)
	{};
}

void CClock::Wait30FPS()
{
	Wait(30);
}

void CClock::Pause(bool bPause)
{
	unsigned long ulDuration;

	if (bPause)
	{
		mulPauseTime = GetTime();
	}
	else
	{
		ulDuration = mulPauseTime - TellStartTime();
		mulClockTime = GetTime() - ulDuration;
	}
}

unsigned long CClock::TellStartTime() const
{
	return mulClockTime;
}

unsigned long CClock::GetTime() const
{
	return GetTickCount();
}
