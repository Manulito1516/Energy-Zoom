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
	mX = 200;
	mY = 0;
	mZ = 300; // position on track
	
	mXRender = 0; // used by render function, do not the mXRender
	mYRender = HORIZON; // used by render function, do not the mYRender
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
	if (g_posZf >= mZ){
		return;
	}
	//mZ no debe ser 0
	mScaleFactor = mZ / (mZ + abs(mZ - g_posZf) * 40); // (mZ - g_posZf) = 0 cuando llega al trigger (no necesariamente a la altura del auto)
	if (mScaleFactor < 0.02 || mScaleFactor > 1){
		return;
	}
	
	// Testing
	mYRender = HORIZON - 5 + (SCREEN_HEIGHT - HORIZON) * mScaleFactor;
	mXRender = mX + (SCREEN_WIDTH - mX)* mScaleFactor;

	
	// render
	mScale.w = 1 + WIDTH * mScaleFactor;
	mScale.h = 1 + HEIGHT * mScaleFactor;
	
	mTexture.render(mXRender, mYRender + mY, &mScale, &mClip);
}

void Obj3D::update(){
	//mX += mSpeedX;
	//mY += mSpeedY;
}
