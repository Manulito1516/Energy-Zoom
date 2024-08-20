#include "bg.h"

#include <SDL2/SDL.h>
//#include <stdio.h>
#include <iosfwd>
#include <string.h>
#include <vector>

#include "../setup.h"

#include "../ME/ME_Texture.h"

// constructor
BG::BG(){
	mX = 0;
	mY = 0;

	mTexturePath = "assets/bg/bg.png";
	mTexture.load(mTexturePath, BG_WIDTH, BG_HEIGHT);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void BG::render(){
	mX = 0;
	while (mX < SCREEN_WIDTH){
		mTexture.render(mX, mY);
		mX += BG_WIDTH;
	}
}
