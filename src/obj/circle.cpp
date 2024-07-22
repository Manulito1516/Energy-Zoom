#include "circle.h"

#include <SDL2/SDL.h>
#include <string.h>
#include <stdio.h>
#include <vector>

#include "../setup.h"

#include "../ME/ME_Texture.h"

// constructor
Circle::Circle(){
	mX = 0;
	mY = 0;
	mScale.x = mX;
	mScale.y = mY;
	mScale.w = 32;
	mScale.h = 32;
	mSpeedX = 0;
	mSpeedY = 0;

	mTexturePath = "assets/circle.png";
	mTexture.load(mTexturePath, CIRCLE_WIDTH, CIRCLE_HEIGHT);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Circle::render(){
  mTexture.render(mX, mY, &mScale);
}

void Circle::update(){
	mX += mSpeedX;
	mY += mSpeedY;
}

void Circle::takeInput(SDL_Event& e){
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mSpeedY -= CIRCLE_SPEED; break;
            case SDLK_s: mSpeedY += CIRCLE_SPEED; break;
            case SDLK_a: mSpeedX -= CIRCLE_SPEED; break;
            case SDLK_d: mSpeedX += CIRCLE_SPEED; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mSpeedY += CIRCLE_SPEED; break;
            case SDLK_s: mSpeedY -= CIRCLE_SPEED; break;
            case SDLK_a: mSpeedX += CIRCLE_SPEED; break;
            case SDLK_d: mSpeedX -= CIRCLE_SPEED; break;
        }
    }
}
