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
	
	SDL_Rect get_rect() const { return mRect; }

private:
	int mPosX, mPosY;
	int mVelX, mVelY;
	
	SDL_Rect mRect;
	
	std::string mTexturePath;
    ME_Texture mTexture;
};

#endif 

