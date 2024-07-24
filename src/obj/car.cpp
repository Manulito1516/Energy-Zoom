#include "car.h"

#include <vector>

#include "../setup.h"
#include "../ME/ME_Texture.h"

Car::Car(){
	mPosX = SCREEN_WIDTH / 2 - CAR_WIDTH / 2;
	mPosY = SCREEN_HEIGHT * (2.00/3.00);

	mVelX = 0;
	mVelY = 0;
	
	mRect.x = mPosX;
	mRect.y = mPosY;
	mRect.w = CAR_WIDTH;
	mRect.h = CAR_HEIGHT;
	
	mTexturePath = "assets/sprites/car.png";
	mTexture.load(mTexturePath, CAR_WIDTH, CAR_HEIGHT);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Car::takeInput(SDL_Event &e){
	//If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch( e.key.keysym.sym ){
			case SDLK_LEFT: mVelX -= CAR_VEL; break;
			case SDLK_RIGHT: mVelX += CAR_VEL; break;
		}
	}
	//If a key was released
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_LEFT: mVelX += CAR_VEL; break;
			case SDLK_RIGHT: mVelX -= CAR_VEL; break;
		}
	}
}

void Car::update(){
	mPosX += mVelX;
	mPosY += mVelY;

	//If the car went too far to the left or right
	if((mPosX < 0) || (mPosX + CAR_WIDTH > SCREEN_WIDTH)){
		//Move back
		mPosX -= mVelX;
	}
	
	mRect.x = mPosX;
	mRect.y = mPosY;
}

void Car::render(){
	mTexture.render(mPosX, mPosY);
}
