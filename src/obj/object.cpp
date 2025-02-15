#include "object.h"

#include <SDL2/SDL.h>
#include <string.h>
//#include <stdio.h>
#include <iosfwd>
#include <vector>

#include "../setup.h"

#include "../ME/ME_Texture.h"

// constructor
Obj_base::Obj_base(){
	mX = 0;
	mY = 0;
	mSpeedX = 0;
	mSpeedY = 0;
	
	mScale.x = 0;
	mScale.y = 0;
	mScale.w = 32;
	mScale.h = 32;
	
	mClip.x = 0;
	mClip.y = 0;
	mClip.w = 32;
	mClip.h = 32;
}

void Obj_base::render(ME_Texture* texture){
  texture->render(mX, mY, &mScale, &mClip);
}

void Obj_base::update(){
	mX += mSpeedX;
	mY += mSpeedY;
}
