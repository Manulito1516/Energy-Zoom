#include "highway.h"

#include <cmath>
#include <stdio.h>
#include <vector>
#include <string.h>

#include "../setup.h"
#include "../ME/ME_Texture.h"

// arreglen los header carajo
const float Highway::HIGHWAY_VEL = 0.01;

Highway::Highway(){	
	mPosX = SCREEN_WIDTH / 2; // Centro
	mPosY = SCREEN_HEIGHT / 2; // de la mitad para abajo
	
	mStartPosY = SCREEN_HEIGHT * (1.00/3.00);

	mVelX = 0;
	mVel = 0.0;
	mAcceleration = 0.0;
	mTurnVel = 0;
	
	//Scale texture rect
	mScale.x = 0;
	mScale.y = 0;
	mScale.w = 2;
	mScale.h = 2; 
	
	//Clip texture rect
	mClip.x = 0;
	mClip.y = 0; // cambiar esto tambien en el while (render)
	mClip.w = HIGHWAY_TEX_W;
	mClip.h = 1; // cambiar esto tambien en el while (render)
	
	// para texturizado por distancia
	mWait = 1; //cuantos ciclos deben pasar para aumentar mClip.y
	mCloseness = 0; // cercania con el PoV
	
	// efectos de avance, perspectiva y giro
	mPosZ = 0;
	mRoadX = -26;
	
	mRoadAngle = mRoadX;
	
	mPosXf = mPosX;
	mWidthf = mScale.w;
	
	mClipYf = 0.5; // posicion del clip.y basada en la velocidad del jugador
	
	mTexturePath = "assets/sprites/highway.png";
	mTexture.load(mTexturePath, HIGHWAY_TEX_W, HIGHWAY_TEX_H);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
}

void Highway::takeInput(SDL_Event &e){
	// If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_LEFT: ++mVelX; break;
			case SDLK_RIGHT: --mVelX; break;
			case SDLK_UP: mAcceleration -= HIGHWAY_VEL; break;
			case SDLK_DOWN: mAcceleration += HIGHWAY_VEL; break;
			case SDLK_a: mTurnVel = -HIGHWAY_VEL; break;
			case SDLK_d: mTurnVel = HIGHWAY_VEL; break;
		}
	}
	//If a key was released
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			case SDLK_LEFT: --mVelX; break;
			case SDLK_RIGHT: ++mVelX; break;
			case SDLK_UP: mAcceleration += HIGHWAY_VEL; break;
			case SDLK_DOWN: mAcceleration -= HIGHWAY_VEL; break;
			case SDLK_a: mTurnVel = 0; break;
			case SDLK_d: mTurnVel = 0; break;
		}
	}
}

void Highway::update(){
	//Move the Highway left or right
	mRoadX += mVelX;
	g_roadTurn += mTurnVel;
	mVel += mAcceleration;
	mClipYf += mVel;
	
	mRoadX -= g_roadTurn * mVel;
}

void Highway::render(){
	mWidthf = 2;
	mPosXf = (SCREEN_WIDTH / 2 - mWidthf) + ((SCREEN_WIDTH) * sin(g_roadTurn));
	
	// Going forward/backwards and looping the texture
	if (mClipYf >= HIGHWAY_TEX_H){ // ATRAS
		mClipYf = 0;
	} else if (mClipYf <= 0){ // ADELANTE
		mClipYf = HIGHWAY_TEX_H;
	}
	mClip.y = (int)mClipYf;
	
	mPosY = mStartPosY;
	
	mWait = 1;
	mCloseness = 1; // cercania
	
	// Como mRoadAngle va a ir cambiando dentro del loop, lo "reinicio" antes de entrar
	mRoadAngle = mRoadX - ((SCREEN_WIDTH / 4) * sin(g_roadTurn));
	while (mPosY < SCREEN_HEIGHT){
		// prepare to render
		mScale.w = (int)mWidthf;
		mPosX = (int)mPosXf;
		
		//render
		mTexture.render(mPosX, mPosY, &mScale, &mClip);
		
		// calculate road angle
		mWidthf += 4* cos(degToRad(mRoadAngle));
		mPosXf += 4* sin(degToRad(mRoadAngle));
		
		// roadTurn
		mRoadAngle += g_roadTurn;
		
		// -----TEXTURIZING based on distance
		--mWait; // wait controla cuantos ciclos deben pasar hasta renderizar la siguiente fila
		if (mWait < 0){
			if (mClip.y < HIGHWAY_TEX_H - 1){
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