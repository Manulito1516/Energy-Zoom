#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <SDL2/SDL.h>
#include <string.h>

#include "../ME/ME_Texture.h"

class Highway{
public:
	static const int HIGHWAY_TEX_W = 16;
	static const int HIGHWAY_TEX_H = 8;
	static const float HIGHWAY_VEL;

	Highway();

	void takeInput(SDL_Event &e);
	void update();
	void render();
	
	float get_posZ() const { return mPosZf; }
	float get_vel() const { return mVel; }
	float get_accel() const { return mAcceleration; }
private:
	int mPosX, mPosY;
	int mStartPosY;
	
	//More resolution i guess
	float mPosXf, mPosZf, mWidthf, mClipYf;

	int mVelX;
	float mTurnVel, mVel, mAcceleration;
	
	// controls of the car
	float mThrottle, mBrake;
	
	//The scale and clip for the render function (drawing in the distance)
	SDL_Rect mScale;
	SDL_Rect mClip;
	
	int mWait, mCloseness; // texturizing
	float mRoadX, mRoadAngle; // perspective of position and turn
	
	std::string mTexturePath;
	ME_Texture mTexture;
};

#endif
