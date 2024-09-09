#include "object3D.h"

#include <SDL2/SDL.h>
#include <string.h>
//#include <stdio.h>
#include <iosfwd>
#include <vector>
#include <cmath>

#include "../setup.h"

#include "../ME/ME_Texture.h"

// constructor
Obj3D::Obj3D(){
	mX = SCREEN_WIDTH / 2 + 128;
	mY = HORIZON;
	mZ = 300; // position on track
	mYoffset = 0;
	mScaleFactor = 1;
	
	mScale.x = 0;
	mScale.y = 0;
	mScale.w = 32;
	mScale.h = 32;
	
	mClip.x = 0;
	mClip.y = 0;
	mClip.w = 32;
	mClip.h = 32;

	mTexturePath = "assets/circle.png";
	mTexture.load(mTexturePath, WIDTH, HEIGHT);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Obj3D::render(){
	//mZ no debe ser 0
	mScaleFactor = mZ / (mZ + abs(mZ - g_posZf) * 30); // (mZ - g_posZf) = 0 cuando llega al trigger (no necesariamente a la altura del auto)
	// unfinished
	mY = SCREEN_HEIGHT * mScaleFactor;

	
	// render
	mScale.w = 1 + WIDTH * mScaleFactor;
	mScale.h = 1 + HEIGHT * mScaleFactor;
	if (mScaleFactor > 0.1){
		mTexture.render(mX, mY + mYoffset, &mScale, &mClip);
	}
	/*if (mScaleFactor == 1){
		//delete
	}*/
}

void Obj3D::update(){
	//mX += mSpeedX;
	//mY += mSpeedY;
}
