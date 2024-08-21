#ifndef GRASS_H
#define GRASS_H

#include <SDL2/SDL.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "../ME/ME_Texture.h"

class Grass{
public:
	static const int GRASS_TEX_W = 1;
	static const int GRASS_TEX_H = 8;

	Grass();

	void render(float lClipYf);
	
private:
	int mPosX, mPosY;
	int mStartPosY;
	
	// The scale and clip for the render function (drawing in the distance)
	SDL_Rect mScale;
	SDL_Rect mClip;
	
	int mWait, mCloseness; // texturizing
	
	std::string mTexturePath;
	ME_Texture mTexture;
};

#endif
