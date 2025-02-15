#ifndef OBJ3D_BASE
#define OBJ3D_BASE

#include <SDL2/SDL.h>
#include <string.h>
#include "../ME/ME_Texture.h"
#include "../ME/ME_Scene.h"
#include "../scenes/GameScene.h"

class GameScene;

class Obj3D{
  public:
    static const int WIDTH = 32;
    static const int HEIGHT = 32;
    
    static const int HORIZON = 80;
    static const int CAR_Y = 160;
    
    Obj3D();
    void create(GameScene* parent);

	//void takeInput(SDL_Event &e);
    void update();
    void render(ME_Texture* texture);
    
    float get_scaleFactor() const { return mScaleFactor; }

  private:
	GameScene* mParent;
    int mX, mY, mYRender;
    float mZ, mXRender, mScaleFactor;
    int mWidth, mHeight;
	SDL_Rect mClip;
    SDL_Rect mScale;
};

#endif
