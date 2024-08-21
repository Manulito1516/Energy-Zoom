#include "grass.h"

//#include <iostream>
//#include <fstream>
#include <cmath>
//#include <stdio.h>
//#include <stdlib.h>
#include <iosfwd>
#include <vector>
#include <string.h>

#include "../setup.h"
#include "../ME/ME_Texture.h"
using namespace std;

Grass::Grass(){	
	mPosX = 0;
	mPosY = SCREEN_HEIGHT / 2; // de la mitad para abajo
	
	mStartPosY = SCREEN_HEIGHT * (1.00/3.00);

	//Scale texture rect
	mScale.x = 0;
	mScale.y = 0;
	mScale.w = SCREEN_WIDTH;
	mScale.h = 2;
	
	//Clip texture rect
	mClip.x = 0;
	mClip.y = 0; // cambiar esto tambien en el while (render)
	mClip.w = GRASS_TEX_W;
	mClip.h = 1; // cambiar esto tambien en el while (render)
	
	// para texturizado por distancia
	mWait = 1; //cuantos ciclos deben pasar para aumentar mClip.y
	mCloseness = 0; // cercania con el PoV
	
	//mClipYf = 0.5; // posicion del clip.y basada en la velocidad del jugador
	
	mTexturePath = "assets/bg/grass.png";
	mTexture.load(mTexturePath, GRASS_TEX_W, GRASS_TEX_H);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Grass::render(float lClipYf){
	
	// Going forward/backwards and looping the texture
	if (lClipYf >= GRASS_TEX_H){ // BACKWARD
		lClipYf = 0;
	} else if (lClipYf <= 0){ // FORWARD
		lClipYf = GRASS_TEX_H;
	}
	mClip.y = (int)lClipYf;
	
	mPosY = mStartPosY;
	
	mWait = 1;
	mCloseness = 1; // cercania
	
	while (mPosY < SCREEN_HEIGHT){
		// render
		mTexture.render(mPosX, mPosY, &mScale, &mClip);
		
		// -----TEXTURIZING based on distance
		--mWait; // wait controla cuantos ciclos deben pasar hasta renderizar la siguiente fila
		if (mWait < 0){
			if (mClip.y < GRASS_TEX_H - 1){
				++mClip.y;
			} else {
				mClip.y = 0;
			}
			
			++mCloseness; // mientras mas cerca del PoV, mas ciclos tarda en llegar a 0
			mWait = mCloseness; // y por lo tanto renderizar la siguiente fila de pixeles
		}
		
		++mPosY; // siguiente fila
	}
}
