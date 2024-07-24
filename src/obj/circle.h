#ifndef OBJ_CIRCLE_H
#define OBJ_CIRCLE_H

#include <SDL2/SDL.h>
#include <string.h>
#include "../ME/ME_Texture.h"

class Circle{
  public:
    Circle();
    static const int CIRCLE_SPEED = 5;
    static const int CIRCLE_WIDTH = 32;
    static const int CIRCLE_HEIGHT = 32;

	void takeInput(SDL_Event &e);
    void update();
    void render();
    
    SDL_Rect get_rect() const { return mRect; }

  private:
    int mX, mY, mSpeedX, mSpeedY;
    int mWidth, mHeight;
    SDL_Rect mScale;
    SDL_Rect mRect;
    
    std::string mTexturePath;
    ME_Texture mTexture;
};

#endif
