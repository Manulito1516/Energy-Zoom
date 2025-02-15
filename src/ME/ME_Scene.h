#ifndef ME_SCENE_H
#define ME_SCENE_H

#include <SDL2/SDL.h>

class ME_Scene {
	public:
		//Scene transitions
		virtual void enter() = 0;
		virtual void exit() = 0;

		//Main loop functions
		virtual void takeInput(SDL_Event &event) = 0;
		virtual void update() = 0;
		virtual void render() = 0;

		//Make sure to call child destructors
		virtual ~ME_Scene(){};
};

#endif
