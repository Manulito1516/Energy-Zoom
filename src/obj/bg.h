#ifndef OBJ_BG
#define OBJ_BG

#include <SDL2/SDL.h>
#include <string.h>
#include "../ME/ME_Texture.h"

class BG{
  public:
	BG();
    static const int BG_WIDTH = 8;
    static const int BG_HEIGHT = 128;
    
    void render(ME_Texture* texture);

  private:
    int mX, mY;
};

#endif
