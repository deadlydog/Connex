// CClock header file (CClock.h)

#ifndef CCLOCK_H
#define CCLOCK_H

class CClock
{
	public:
		
		CClock();
		void Start();
		void Wait(unsigned long WaitTime);
		void Wait30FPS();
		void Pause(bool bPause = true);

		unsigned long TellStartTime() const;
		unsigned long GetTime() const;

	private:

		unsigned long mulClockTime;
		unsigned long mulPauseTime;
};

#endif