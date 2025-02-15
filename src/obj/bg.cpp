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
}

void BG::render(ME_Texture* texture){
	mX = 0;
	while (mX < SCREEN_WIDTH){
		texture->render(mX, mY);
		mX += BG_WIDTH;
	}
}
