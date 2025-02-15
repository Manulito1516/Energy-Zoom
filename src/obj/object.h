#ifndef OBJ_BASE
#define OBJ_BASE

#include <SDL2/SDL.h>
#include <string.h>
#include "../ME/ME_Texture.h"

class Obj_base{
  public:
	Obj_base();
    static const int OBJ_WIDTH = 32;
    static const int OBJ_HEIGHT = 32;

	//void takeInput(SDL_Event &e);
    void update();
    void render(ME_Texture* texture);

  private:
    int mX, mY, mSpeedX, mSpeedY;
    int mWidth, mHeight;
	SDL_Rect mClip;
    SDL_Rect mScale;
};

#endif
