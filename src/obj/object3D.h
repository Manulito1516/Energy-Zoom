#ifndef OBJ3D_BASE
#define OBJ3D_BASE

#include <SDL2/SDL.h>
#include <string.h>
#include "../ME/ME_Texture.h"

class Obj3D{
  public:
	Obj3D();
    static const int WIDTH = 32;
    static const int HEIGHT = 32;
    
    static const int HORIZON = 80;
    static const int CAR_Y = 160;

	//void takeInput(SDL_Event &e);
    void update();
    void render();
    
    float get_scaleFactor() const { return mScaleFactor; }

  private:
    int mX, mY, mYRender;
    float mZ, mXRender, mScaleFactor;
    int mWidth, mHeight;
	SDL_Rect mClip;
    SDL_Rect mScale;
    std::string mTexturePath;
    ME_Texture mTexture;
};

#endif
