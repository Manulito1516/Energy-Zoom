#include "highway.h"

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

// arreglen los header carajo
const float Highway::HIGHWAY_VEL = 0.01;
const float Highway::HIGHWAY_MAX_VEL = 0.85;

Highway::Highway(){	
	mPosX = SCREEN_WIDTH / 2; // Centro
	mPosY = SCREEN_HEIGHT / 2; // de la mitad para abajo
	
	mStartPosY = SCREEN_HEIGHT / 3;

	// Positions and velocity
	//g_posZf = 0.0;
	g_vel = 0.0;
	mAcceleration = 0.0;
	mVelXf = 0;
	mTurnVel = 0;
	mMaxVelocity = HIGHWAY_MAX_VEL;
	
	// Controls
	mThrottle = 0.0;
	mBrake = 0.0;

	// Read road
	mTriggerNumber = 1;
	mTriggerPos = 0;
	mTriggerTurnTarget = 0.0;
	mTriggerTurnSpeed = 0.0;
	mTriggerLoopPos = 99999;
	
	// Hitbox
	mHitbox.x = 0;
	mHitbox.y = SCREEN_HEIGHT * (2.00/3.00);
	mHitbox.w = SCREEN_WIDTH + 70;
	mHitbox.h = 32;

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
	g_roadX = -26;
	
	g_roadAngle = g_roadX;
	
	mPosXf = mPosX;
	mWidthf = mScale.w;
	
	mClipYf = 0.5; // posicion del clip.y basada en la velocidad del jugador
	
	mTexturePath = "assets/sprites/highway.png";
	mTexture.load(mTexturePath, HIGHWAY_TEX_W, HIGHWAY_TEX_H);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
	
	g_track.seekg(0, ifstream::beg);
	loadTrigger();
}

void Highway::takeInput(SDL_Event &e){
	// If a key was pressed
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			// inputs
			case SDLK_LEFT: mVelXf += 0.5; break;
			case SDLK_RIGHT: mVelXf -= 0.5; break;
			case SDLK_UP: mThrottle += HIGHWAY_VEL; break;
			case SDLK_DOWN: mBrake += HIGHWAY_VEL * 150; break;
			//case SDLK_a: mTurnVel = -HIGHWAY_VEL; break;
			//case SDLK_d: mTurnVel = HIGHWAY_VEL; break;
		}
	}
	//If a key was released
	else if(e.type == SDL_KEYUP && e.key.repeat == 0){
		switch(e.key.keysym.sym){
			// evil inputs (release)
			case SDLK_LEFT: mVelXf -= 0.5; break;
			case SDLK_RIGHT: mVelXf += 0.5; break;
			case SDLK_UP: mThrottle -= HIGHWAY_VEL; break;
			case SDLK_DOWN: mBrake -= HIGHWAY_VEL * 150; break;
			//case SDLK_a: mTurnVel = 0; break;
			//case SDLK_d: mTurnVel = 0; break;
		}
	}
}

void Highway::update(){
	//Move the Highway left or right
	g_roadX += mVelXf * (g_vel/HIGHWAY_MAX_VEL);
	
	//Road turns
	if (g_vel > 0) {
		g_roadTurn += mTurnVel;
	}
	
	// Acceleration and Z axis things
	if (g_vel < mMaxVelocity){
		mAcceleration = mThrottle / 2; // is this temporary?
		g_vel += mAcceleration;
	}
	
	// "Friction", "air resistance", yeah, you get it
	if (g_vel > 0){
		g_vel -= HIGHWAY_VEL / 4 * (1+mBrake);
	} else if (g_vel < 0){
		g_vel += HIGHWAY_VEL / 4 * (1+mBrake);
	}
	
	// Clip min values
	if (g_vel < 0.001 || g_vel > 3){ // realistically speaking, 3 is too fast
		g_vel = 0;
	}
	
	// Position (DON'T MODIFY DIRECTLY)
	mClipYf -= g_vel; // controls the texture side (note that this is negative)
	g_posZf += g_vel; // controls the position on circuit
	
	// Moves the car out if it goes straight during a turn
	if (g_vel > HIGHWAY_MAX_VEL / 4){
		g_roadX += g_roadTurn * (g_vel + mThrottle * 80) * 0.9; // higher number means lower grip
	}
	
	// Hitbox
	mHitbox.x = (g_roadX + 24) *9 + SCREEN_WIDTH * sin(g_roadTurn) * 2.5 - (mHitbox.w - SCREEN_WIDTH) / 2;//- SCREEN_WIDTH / 2;
	
	// *** COLLISIONS ***
	// road
	if (g_onRoad){
		//on road
		mMaxVelocity = HIGHWAY_MAX_VEL; 
	} else {
		//out of the road
		//g_vel -= HIGHWAY_VEL * 4;
		mMaxVelocity = HIGHWAY_MAX_VEL / 4;
	}
}

void Highway::readRoad(){
	if (g_posZf > mTriggerPos){ // after trigger position
		if (mTriggerPos == 0){
			g_track.seekg(0, ifstream::beg);
			loadTrigger();
		}
		
		// first check positivity, then if it's not enough...
		if (mTriggerTurnTarget >= 0 && g_roadTurn < mTriggerTurnTarget) {
			mTurnVel = mTriggerTurnSpeed * (1+g_vel); // turns right
			
		// check negativity, if it's not enough...
		} else if (mTriggerTurnTarget <= 0 && g_roadTurn > mTriggerTurnTarget) {
			mTurnVel = -mTriggerTurnSpeed * (1+g_vel); // turns left
		} else {
			mTurnVel = 0;
		}
		
		// return to straight
		if (mTriggerTurnTarget == 0 && g_roadTurn > -0.01 && g_roadTurn < 0.01) {
			mTurnVel = 0;
			g_roadTurn = 0;
		}
		
		// same case just after :)
		if ((mTriggerTurnTarget > 0 && g_roadTurn > mTriggerTurnTarget)
		or (mTriggerTurnTarget == 0 && g_roadTurn == 0 && mTriggerPos != 0)
		or (mTriggerTurnTarget < 0 && g_roadTurn < mTriggerTurnTarget)) {
			mTriggerNumber++;
			loadTrigger();
		} 
	}
	
	if (g_posZf > mTriggerLoopPos){
		// temp
		mTriggerNumber = 1;
		g_posZf = 0;

		reloadTrack();
		loadTrigger();
	}
}

void Highway::loadTrigger(){
	if (!g_track.is_open()){
		return;
	}
	
	// make temp strings
	string sTriggerPos;
	string sTriggerTurnTarget;
	string sTriggerTurnPos;
	
	// get the data from file g_track in strings
	getline(g_track, sTriggerPos, ',');
	getline(g_track, sTriggerTurnTarget, ',');
	getline(g_track, sTriggerTurnPos, ';');
	
	// string to int, string to double (float)
	mTriggerPos = atoi(sTriggerPos.c_str());
	mTriggerTurnTarget = (float)atof(sTriggerTurnTarget.c_str());
	mTriggerTurnSpeed = (float)atof(sTriggerTurnPos.c_str());
	
	if (mTriggerPos == 0) {
		mTriggerLoopPos = (int)g_posZf;
	}
}

void Highway::render(){
	mWidthf = 2;
	mPosXf = (SCREEN_WIDTH / 2 - mWidthf) + ((SCREEN_WIDTH) * sin(g_roadTurn));
	
	// Going forward/backwards and looping the texture
	if (mClipYf >= HIGHWAY_TEX_H){ // BACKWARD
		mClipYf = 0;
	} else if (mClipYf <= 0){ // FORWARD
		mClipYf = HIGHWAY_TEX_H;
	}
	mClip.y = (int)mClipYf;
	
	mPosY = mStartPosY;
	
	mWait = 1;
	mCloseness = 1; // cercania
	
	// Como g_roadAngle va a ir cambiando dentro del loop, lo "reinicio" antes de entrar
	g_roadAngle = g_roadX - ((SCREEN_WIDTH / 4) * sin(degToRad(g_roadTurn)));
	while (mPosY < SCREEN_HEIGHT){
		
		// prepare to render
		mScale.w = (int)mWidthf;
		mPosX = (int)mPosXf;
		
		//***************** RENDER *****************
		mTexture.render(mPosX, mPosY, &mScale, &mClip);
		//***************** RENDER *****************
		
		// calculate road angle
		mWidthf += 4* cos(degToRad(g_roadAngle));
		mPosXf += 4* sin(degToRad(g_roadAngle));
		
		// roadTurn
		g_roadAngle += g_roadTurn;
		
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
	
	// hitbox rendering
	//g_hitboxTexture.render(mHitbox.x, mHitbox.y, &mHitbox);
}
