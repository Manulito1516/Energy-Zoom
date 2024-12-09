#include "setup.h"
#include "obj/circle.h"
#include "obj/car.h"
#include "obj/highway.h"
#include "obj/bg.h"
#include "obj/grass.h"
#include "obj/object3D.h"

//#include "obj/object.h"
#include "ME/ME_Timer.h"

int main(int argc, char* args[]){
	if (init()){
		if (loadAssets()){
			bool quit = false;
			
			SDL_Event event;
			
			// *** INITIALIZE OBJECTS ***
			Circle o_circle;
			Highway o_highway;
			Car o_car;
			BG o_bg;
			Grass o_grass;
			Obj3D o_obj3d;
			//Obj_base obj;
			
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
					o_circle.takeInput(event);
					o_highway.takeInput(event);
					o_car.takeInput(event);
				}
				
				//Calculate and correct fps
				int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if(avgFPS > 2000000){
					avgFPS = 0;
				}
				
				// *** CHECK COLLISIONS ***
				g_onRoad = checkCollision(o_highway.get_hitbox(), o_car.get_hitbox());
				
				// *** UPDATE OBJECTS ***
				o_circle.update();
				o_highway.update();
				o_highway.readRoad();
				o_car.update();
				//obj.update();
				
				// FPS COUNTER
				timeText = "FPS: " + std::to_string(avgFPS);
				int numbar = 10;
				std::string debugTexts[numbar] = {
					"g_posZ: " + std::to_string(g_posZf),
					"highway.mVel: " + std::to_string(g_vel),
					"highway.mAccel: " + std::to_string(o_highway.get_accel()),
					"g_roadTurn: " + std::to_string(g_roadTurn),
					"mNextTrigger: " + std::to_string(o_highway.get_nextTrigger()),
					"mTriggerPos: " + std::to_string(o_highway.get_triggerPos()),
					"onRoad: " + std::to_string(g_onRoad),
					"cardir: " + std::to_string(o_car.get_dir()),
					"mRoadX " + std::to_string(o_highway.get_roadX()),
					"mScaleFactor " + std::to_string(o_obj3d.get_scaleFactor())
				};
				
				// clear screen
				SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(g_renderer);
				
				// *** RENDER OBJECTS ***
				// render background
				o_bg.render();
				// objects
				o_grass.render(o_highway.get_clipYf());
				o_highway.render();
				o_car.render();
				o_circle.render();
				o_obj3d.render();
				//obj.render();
				
				g_manusFont.renderText(0,0, timeText);
				
				for (int i = 0; i < numbar; i++){
					g_manusFont.renderText(0, 12 + i * 12, debugTexts[i]);
				}
				
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
