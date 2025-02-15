#include "setup.h"

//#include "obj/object.h"
#include "ME/ME_Scene.h"
#include "ME/ME_Timer.h"

#include "scenes/TitleScene.h"

int main(int argc, char* args[]){
	if (init()){
		if (loadAssets()){
			bool quit = false;
			
			SDL_Event event;
			
			// *** INITIALIZE OBJECTS ***
			g_currentScene = TitleScene::get();
			g_currentScene->enter();
			
			//SDL_Color textColor = {255, 255, 255, 255};
			ME_Timer fpsTimer;
			ME_Timer capTimer;
			std::string timeText;
			
			int countedFrames = 0;
			fpsTimer.start();
			
			// runtime
			while (!quit){
				capTimer.start();
				while (SDL_PollEvent(&event) != 0){
					
					// quit event
					if (event.type == SDL_QUIT){
						quit = true;
					}
					
					if (event.type == SDL_KEYDOWN){
						if (event.key.keysym.sym == SDLK_F11){
							if (g_fullscreen){
								
								SDL_SetWindowFullscreen(g_window, 0);
								g_fullscreen = false;
								
							} else {
								if (g_soft_render){
									SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN); //actual fullscreen
								} else {
									SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
								}
								
								g_fullscreen = true;
							}
						}
					}
					g_currentScene->takeInput(event);
				}
				
				//Calculate and correct fps
				int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if(avgFPS > 2000000){
					avgFPS = 0;
				}
				
				// *** CHECK COLLISIONS ***
				//g_onRoad = checkCollision(o_highway.get_hitbox(), o_car.get_hitbox());
				
				// *** UPDATE OBJECTS ***
				g_currentScene->update();
				
				// FPS COUNTER
				timeText = "FPS: " + std::to_string(avgFPS);
				
				changeScene();
				
				// clear screen
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(g_renderer);
				
				// *** RENDER OBJECTS ***
				g_currentScene->render();
				
				g_manusFont.renderText(0,0, timeText);
				
				// update screen
				SDL_RenderPresent(g_renderer);
				++countedFrames;
				
				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICK_PER_FRAME){
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}
			}
		} else { printf("error at loadAssets() \n"); }
	} else { printf("error at init() \n"); }
	
	close();
	return 0;
}
