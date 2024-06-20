#ifndef SETUP_H
#define SETUP_H

#include "ME/ME_Texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>


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

// global texture list and font
extern std::vector<ME_Texture*> g_textures;

const int g_fontSize = 10;
const int g_fontSpacing = 8;
extern ME_Texture g_fontTexture;

// Road globals
extern float g_roadTurn;

//-----------------------
// functions

// define functions
extern bool init();
extern void close();
extern bool loadAssets();
extern float degToRad(float degrees);

#endif
