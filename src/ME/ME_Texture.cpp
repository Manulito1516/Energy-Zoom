#include <stdio.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ME_Texture.h"
#include "../setup.h"

ME_Texture::ME_Texture(){ // constructor
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

ME_Texture::~ME_Texture(){ // destructor
	free();
}

bool ME_Texture::load(std::string path, int w, int h){
	free(); // if it already had a texture

	SDL_Texture *texture = IMG_LoadTexture(g_renderer, path.c_str());
	if( texture == NULL ){
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}

	// Get texture size
	mWidth = w;
	mHeight = h;

	//Return success
	mTexture = texture;
	return mTexture != NULL;
}

void ME_Texture::renderText(int x, int y, std::string stringText){
	std::string textureText = stringText.c_str();
	//SDL_Surface *textSurface = SDL_CreateRGBSurfaceWithFormat(0, g_fontSpacing * stringText.size(), g_fontSize, 0, SDL_PIXELFORMAT_ABGR8888);
	//SDL_Surface *textSurface = IMG_Load("assets/manus.png");
	SDL_Rect srcRect; //source rect
	SDL_Rect dstRect; //destination rect
	srcRect.w = g_fontSpacing;
	srcRect.h = g_fontSize;
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = g_fontSpacing;
	dstRect.h = g_fontSize;
	
	for (int i = 0; i < textureText.size(); i++){
		srcRect.x = 0; // you MUST reset the x,
		srcRect.y = 0; // the entire string is printed in this 'for'
		// special case: space
		if (textureText[i] == 32){
			// space character uses DEL char in img
			srcRect.x = g_fontSpacing*(textureText[i]-95); 
		} else {
			// any other character (the image starts from the ! char)
			srcRect.x = g_fontSpacing*(textureText[i]-33);
		}
		// This is like reading, after this comment you will continue reading below.
		// While it's out of bounds, go back 16 chars in X and go one char down in Y
		// which basically means a change of line.
		while (srcRect.x >= (g_fontSpacing * 16)){
			srcRect.y += g_fontSize;
			srcRect.x -= g_fontSpacing * 16;
		}
		
		SDL_RenderCopy(g_renderer, mTexture, &srcRect, &dstRect); // print a character
		
		// so it doesn't print in the same place next time
		dstRect.x += g_fontSpacing;
	}
}

void ME_Texture::free(){
	//Free texture if it exists
	if(mTexture != NULL){
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void ME_Texture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void ME_Texture::setBlendMode(SDL_BlendMode blending){
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void ME_Texture::setAlpha(Uint8 alpha){
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void ME_Texture::render(int x, int y, SDL_Rect* scale, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};

	//Set clip rendering dimensions
	if (clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	if (scale != NULL){
		renderQuad.w = scale->w;
		renderQuad.h = scale->h;
	}
	//Render to screen
	SDL_RenderCopyEx(g_renderer, mTexture, clip, &renderQuad, angle, center, flip);
}


int ME_Texture::getWidth(){
	return mWidth;
}

int ME_Texture::getHeight(){
	return mHeight;
}
