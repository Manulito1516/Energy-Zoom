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
	mX = 0;
	mY = 80;
	mZ = 300; // position on track
	mYoffset = 0;
	
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
  int scaleFactor = 1;
  // no checkeado, mZ no debe ser 0
  scaleFactor = mZ / (mZ + abs(mZ - g_posZf)) // (mZ - g_posZf) = 0 cuando llega al trigger (no necesariamente a la altura del auto)
  
  
  // render
  mScale.x = WIDTH * scaleFactor;
  mScale.y = HEIGHT * scaleFactor;
  mTexture.render(mX, mY + mYoffset, &mScale, &mClip);
}

void Obj3D::update(){
	//mX += mSpeedX;
	//mY += mSpeedY;
}
