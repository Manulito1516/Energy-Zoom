#ifndef CAR_H
#define CAR_H

#include <SDL2/SDL.h>
#include <string.h>

#include "../ME/ME_Texture.h"
#include "../ME/ME_Scene.h"
#include "../scenes/GameScene.h"

class GameScene;

class Car{
public:
	static const int CAR_WIDTH = 32;
	static const int CAR_HEIGHT = 32;

	Car();
	void create(GameScene* parent);

	void takeInput(SDL_Event &e);
	void update();
	void render(ME_Texture* texture);
	
	SDL_Rect get_hitbox() const { return mHitbox; }
	int get_dir() const { return mDir; }

private:
	GameScene* mParent;
	int mPosX, mPosY;
	int mDir, mDirDir;
	bool mAccelerating;
	
	bool mFlip;
	void setSprite(int a, int b, bool flip);
	
	SDL_Rect mHitbox, mClip;
};

#endif 

