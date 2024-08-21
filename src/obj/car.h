#ifndef CAR_H
#define CAR_H

#include <SDL2/SDL.h>
#include <string.h>

#include "../ME/ME_Texture.h"

class Car{
public:
	static const int CAR_WIDTH = 32;
	static const int CAR_HEIGHT = 32;

	Car();

	void takeInput(SDL_Event &e);
	void update();
	void render();
	
	SDL_Rect get_hitbox() const { return mHitbox; }
	int get_dir() const { return mDir; }

private:
	int mPosX, mPosY;
	int mDir, mDirDir;
	bool mAccelerating;
	
	bool mFlip;
	void setSprite(int a, int b, bool flip);
	
	SDL_Rect mHitbox, mClip;
	
	
	std::string mTexturePath;
    ME_Texture mTexture;
};

#endif 

