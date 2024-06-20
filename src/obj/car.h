#ifndef CAR_H
#define CAR_H

#include <SDL2/SDL.h>
#include <string.h>

#include "../ME/ME_Texture.h"

class Car{
public:
	static const int CAR_WIDTH = 32;
	static const int CAR_HEIGHT = 32;

	static const int CAR_VEL = 0;

	Car();

	void takeInput(SDL_Event &e);
	void update();
	void render();

private:
	int mPosX, mPosY;
	int mVelX, mVelY;
	
	std::string mTexturePath;
    ME_Texture mTexture;
};

#endif 

