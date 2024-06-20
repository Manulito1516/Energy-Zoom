#ifndef ME_TIMER_H
#define ME_TIMER_H

#include <stdio.h>

class ME_Timer {
	public:
		// Constructor
		ME_Timer();

		// Timer functions
		void start();
		void stop();
		void pause();
		void unpause();
		
		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		// Clock ticks when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

#endif
