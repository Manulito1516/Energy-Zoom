#include "setup.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ME/ME_Texture.h"
#include "ME/ME_Timer.h"

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
bool g_fullscreen = false;
bool g_soft_render = false;

ME_Texture g_manusFont;

// vector (dinamic array) where the textures will be listed
std::vector<ME_Texture*> g_textures;

// Road
float g_roadTurn = 0;

// ---------------------------------------------
// functions
// init and close
bool init(){
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) >= 0){
		
		//printf("software renderer? (1=yes 0=no)");
		//std::cin >> g_soft_render;
		g_soft_render = 0;
		
		// Create window
		g_window = SDL_CreateWindow("Energy-Zoom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (g_window != NULL){
			if (g_soft_render){
				g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
			} else {
				g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			}
				
			SDL_RenderSetLogicalSize(g_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (g_renderer != NULL){
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);

				// Initialize PNG loading
				IMG_Init(IMG_INIT_PNG & IMG_INIT_PNG);

			} else { success = false; }
		} else { success = false; }
	} else { success = false; }

	if (success == false){
		printf("SDL Error: %s\n", SDL_GetError());
	}

  return success;
}

void close(){
	// close font (ME_Texture)
	g_manusFont.free();
	
	//Free the textures
	for (int i = 0; i < g_textures.size(); ++i){
		g_textures[i]->free();
	}
	//Destroy renderer and window
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_renderer = NULL;
	g_window = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

// assets
bool loadAssets(){
	bool success = true;
	g_manusFont.load("assets/manus.png", 128, 64);
	
	return success;
}

// function that convert degrees to radians
float degToRad(float degrees){
	return degrees * (M_PI / 180);
}
