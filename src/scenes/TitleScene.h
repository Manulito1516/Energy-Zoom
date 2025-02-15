#ifndef ME_TITLESCENE_H
#define ME_TITLESCENE_H

#include "../ME/ME_Scene.h"
#include "../ME/ME_Texture.h"

class TitleScene : public ME_Scene {
	public:
		//Static accessor
		static TitleScene* get();

		//Transitions
		void enter();
		void exit();

		//Main loop functions
		void takeInput(SDL_Event &event);
		void update();
		void render();

	private:
		//Static instance
		static TitleScene sTitleScene;

		//Private constructor
		TitleScene();

		// Title image
		ME_Texture mTitleTexture;
};

#endif
