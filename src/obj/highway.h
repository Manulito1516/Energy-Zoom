#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <SDL2/SDL.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "../ME/ME_Texture.h"

class Highway{
public:
	static const int HIGHWAY_TEX_W = 64;
	static const int HIGHWAY_TEX_H = 8;
	static const float HIGHWAY_VEL;
	static const float HIGHWAY_MAX_VEL;

	Highway();

	void takeInput(SDL_Event &e);
	void update();
	void readRoad();
	void render();
	
	void loadTrigger();
	
	float get_clipYf() const { return mClipYf; }
	SDL_Rect get_hitbox() const { return mHitbox; }
	
	// debug
	float get_accel() const { return mAcceleration; }
	int get_nextTrigger() const { return mTriggerNumber; }
	int get_triggerPos() const { return mTriggerPos; }
	
private:
	int mPosX, mPosY;
	int mStartPosY;
	
	// More resolution i guess
	float mPosXf, mVelXf, mWidthf, mClipYf;
	float mTurnVel, mAcceleration, mMaxVelocity;
	//float mRoadAngle;
	
	// controls of the car
	float mThrottle, mBrake;
	
	// read road
	int mTriggerNumber, mTriggerPos, mTriggerLoopPos;
	float mTriggerTurnSpeed, mTriggerTurnTarget;
	
	//std::ifstream Track;
	
	
	// The scale and clip for the render function (drawing in the distance)
	SDL_Rect mScale;
	SDL_Rect mClip;
	
	// la caja misteriosa [?]
	SDL_Rect mHitbox;
	
	int mWait, mCloseness; // texturizing
	
	std::string mTexturePath;
	ME_Texture mTexture;
};

#endif
