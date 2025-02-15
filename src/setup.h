#ifndef SETUP_H
#define SETUP_H

#include "ME/ME_Texture.h"
#include "ME/ME_Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
//#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;


// SCREEN SIZE
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//-----------------------
// external

// window & renderer
extern SDL_Window *g_window;
extern SDL_Renderer *g_renderer;
extern bool g_fullscreen;
extern bool g_soft_render;

// font and global textures
const int g_fontSize = 10;
const int g_fontSpacing = 8;
extern ME_Texture g_manusFont;
extern ME_Texture g_hitboxTexture;

// scenes
extern ME_Scene* g_currentScene;
extern ME_Scene* g_nextScene;

//-----------------------
// functions

// scenes
extern void setNextScene(ME_Scene* nextScene);
extern void changeScene();

extern bool init();
extern void close();
extern bool loadAssets();
extern float degToRad(float degrees);
extern void reloadTrack();
extern bool checkCollision(SDL_Rect RectA, SDL_Rect RectB);

#endif
