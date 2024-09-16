#include "car.h"

#include <vector>

#include "../setup.h"
#include "../ME/ME_Texture.h"

Car::Car(){
	mPosX = SCREEN_WIDTH / 2 - CAR_WIDTH / 2;
	mPosY = SCREEN_HEIGHT * (2.00/3.00); // 160

	mDir = 0;
	mDirDir = 0;
	mAccelerating = false;
	
	mHitbox.x = mPosX;
	mHitbox.y = mPosY;
	mHitbox.w = CAR_WIDTH;
	mHitbox.h = CAR_HEIGHT;
	
	mClip.x = 0;
	mClip.y = 0;
	mClip.w = 32; // fijo
	mClip.h = 32; // fijo
	mFlip = false;
	
	mTexturePath = "assets/sprites/carsheet.png";
	mTexture.load(mTexturePath, CAR_WIDTH, CAR_HEIGHT);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Car::takeInput(SDL_Event &e){
	//If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch( e.key.keysym.sym ){
			case SDLK_LEFT: mDir--; break;
			case SDLK_RIGHT: mDir++; break;
			case SDLK_UP: mAccelerating = true; break;
		}
	}
	//If a key was released
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_LEFT: mDir++; break;
			case SDLK_RIGHT: mDir--; break;
			case SDLK_UP: mAccelerating = false; break;
		}
	}
}

void Car::update(){
	// mDir can be -1, 0 or 1
	// mDirDir goes from -10 to 10
	
	// limit mDirDir
	if (mDirDir < 15 && mDirDir > -15){
		mDirDir += mDir;
	}
	
	// go back to straight
	if (mDir == 0 or ((mDirDir > 0 and mDirDir < 0) or (mDirDir < 0 and mDirDir > 0))){
		if (mDirDir < 0){
			mDirDir++;
		} else if (mDirDir > 0){
			mDirDir--;
		}
	}
	// DIRDIR
	// left
	if (mDirDir > 14 && !mAccelerating) {
		setSprite(2, 0, true);
		
	} else if (mDirDir > 4 && mDir > 0) {
		setSprite(1, 0, true);
		
	// right
	} else if (mDirDir < -14 && !mAccelerating) {
		setSprite(2, 0, false);
		
	} else if (mDirDir < -4 && mDir < 0) {
		setSprite(1, 0, false);
	
	// center
	} else {
		setSprite(0, 0, false);
	}
}

void Car::setSprite(int a, int b, bool flip = false){
	mClip.x = a*32;
	mClip.y = b*32;
	if (flip){
		mFlip = SDL_FLIP_HORIZONTAL;
	} else {
		mFlip = NULL;
	}
}

void Car::render(){
	if (mFlip) {
		mTexture.render(mPosX, mPosY, NULL, &mClip, 0.0, NULL, SDL_FLIP_HORIZONTAL);
	} else {
		mTexture.render(mPosX, mPosY, NULL, &mClip);
	}
}
