#include <stdio.h>
#include <SDL2/SDL.h>
#include "ME_Timer.h"

ME_Timer::ME_Timer(){
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void ME_Timer::start(){
    mStarted = true;
    mPaused = false;

    // Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void ME_Timer::stop(){
    mStarted = false;
    mPaused = false;

	mStartTicks = 0;
	mPausedTicks = 0;
}

void ME_Timer::pause(){
    if( mStarted && !mPaused )
    {
        mPaused = true;

        // Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void ME_Timer::unpause(){
	if(mStarted && mPaused ){
		mPaused = false;
		
		// Reset ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

Uint32 ME_Timer::getTicks(){
	// The actual timer time
	Uint32 time = 0;

    if (mStarted){
		if (mPaused){
			
			// Return the number of ticks when the timer was paused
			time = mPausedTicks;
		} else {
			
            // Return the ticks of the timer
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool ME_Timer::isStarted(){
	// Returns true if timer is running
    return mStarted;
}

bool ME_Timer::isPaused(){
	// Returns true if timer is running and paused
    return mPaused && mStarted;
}
