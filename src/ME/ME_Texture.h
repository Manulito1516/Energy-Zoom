#ifndef ME_TEXTURE_H
#define ME_TEXTURE_H

#include <string>

// go to hell
#include <SDL2/SDL.h>

class ME_Texture {
	public:
		// Constructor, destructor
		ME_Texture();
		~ME_Texture();

		// Create the MEngine Texture
		bool load(std::string path, int w, int h);

		// Things you can do to the texture
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* scale = NULL, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void renderText(int x, int y, std::string stringText);
		
		// Getters
		int getWidth();
		int getHeight();

	private:
		SDL_Texture *mTexture = NULL;
		int mWidth = 0;
		int mHeight = 0;
};

#endif
