#include "highway.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
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
	
	mStartPosY = SCREEN_HEIGHT * (1.00/3.00);

	// Positions and velocity
	mPosZf = 0.0;
	mVel = 0.0;
	mAcceleration = 0.0;
	mVelXf = 0;
	mTurnVel = 0;
	
	// Controls
	mThrottle = 0.0;
	mBrake = 0.0;

	// Read road
	mTriggerNumber = 1;
	mTriggerPos = 0;
	mTriggerTurnTarget = 0.0;
	mTriggerTurnSpeed = 0.0;
	mTriggerLoopPos = 99999;
	
	//ifstream Track;
	//Track.open("tracks/oval");

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
	mRoadX = -26;
	
	mRoadAngle = mRoadX;
	
	mPosXf = mPosX;
	mWidthf = mScale.w;
	
	mClipYf = 0.5; // posicion del clip.y basada en la velocidad del jugador
	
	mTexturePath = "assets/sprites/highway.png";
	mTexture.load(mTexturePath, HIGHWAY_TEX_W, HIGHWAY_TEX_H);
	g_textures.push_back(&mTexture); // add to the end of the vector/array
	
	Track.seekg(0, ifstream::beg);
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
			case SDLK_DOWN: mBrake += HIGHWAY_VEL * 100; break;
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
			case SDLK_DOWN: mBrake -= HIGHWAY_VEL * 100; break;
			//case SDLK_a: mTurnVel = 0; break;
			//case SDLK_d: mTurnVel = 0; break;
		}
	}
}

void Highway::update(){
	//Move the Highway left or right
	mRoadX += mVelXf * (mVel/HIGHWAY_MAX_VEL);
	g_roadTurn += mTurnVel;
	
	// Acceleration and Z axis things
	if (mVel < HIGHWAY_MAX_VEL){
		mAcceleration = mThrottle / 2; // is this temporary?
		mVel += mAcceleration;
	}
	
	// "Friction", "air resistance", yeah, you get it
	if (mVel > 0){
		mVel -= HIGHWAY_VEL / 4 * (1+mBrake);
	} else if (mVel < 0){
		mVel += HIGHWAY_VEL / 4 * (1+mBrake);
	}
	
	// Clip min values
	if (mVel < 0.001 || mVel > 3){ // realistically speaking, 3 is too fast
		mVel = 0;
	}
	
	// Position (DON'T MODIFY DIRECTLY)
	mClipYf += -mVel; // controls the texture side (note that this is negative)
	mPosZf += mVel; // controls the position on circuit
	
	// Moves the car out if it goes straight during a turn
	mRoadX += g_roadTurn * mVel * 2; // higher number means lower grip
}

void Highway::readRoad(){
	if (mPosZf > mTriggerPos){ // after trigger position
		if (mTriggerPos == 0){
			Track.seekg(0, ifstream::beg);
			loadTrigger();
		}
		
		// first check positivity, then if it's not enough...
		if (mTriggerTurnTarget >= 0 && g_roadTurn < mTriggerTurnTarget) {
			mTurnVel = mTriggerTurnSpeed * (1+mVel); // turns right
			
		// check negativity, if it's not enough...
		} else if (mTriggerTurnTarget <= 0 && g_roadTurn > mTriggerTurnTarget) {
			mTurnVel = -mTriggerTurnSpeed * (1+mVel); // turns left
		} else {
			mTurnVel = 0;
		}
		
		if (mTriggerTurnTarget == 0 && g_roadTurn > -0.01 && g_roadTurn < 0.01) {
			mTurnVel = 0;
			g_roadTurn = 0;
		}
		
		// same case just after :)
		if (mTriggerTurnTarget > 0 && g_roadTurn > mTriggerTurnTarget
		or mTriggerTurnTarget == 0 && g_roadTurn == 0 && mTriggerPos != 0
		or mTriggerTurnTarget < 0 && g_roadTurn < mTriggerTurnTarget) {
			mTriggerNumber++;
			loadTrigger();
		} 
	}
	
	if (mPosZf > mTriggerLoopPos){
		// temp
		mTriggerNumber = 1;
		mPosZf = 0;

		reloadTrack();
		loadTrigger();
	}
}

void Highway::loadTrigger(){
	if (!Track.is_open()){
		return;
	}
	
	// make temp strings
	string sTriggerPos;
	string sTriggerTurnTarget;
	string sTriggerTurnPos;
	
	// save the data from file Track to strings
	getline(Track, sTriggerPos, ',');
	getline(Track, sTriggerTurnTarget, ',');
	getline(Track, sTriggerTurnPos, ';');
	
	// string to int, string to double (float)
	mTriggerPos = atoi(sTriggerPos.c_str());
	mTriggerTurnTarget = (float)atof(sTriggerTurnTarget.c_str());
	mTriggerTurnSpeed = (float)atof(sTriggerTurnPos.c_str());
	
	if (mTriggerPos == 0) {
		mTriggerLoopPos = (int)mPosZf;
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
	
	// Como mRoadAngle va a ir cambiando dentro del loop, lo "reinicio" antes de entrar
	mRoadAngle = mRoadX - ((SCREEN_WIDTH / 4) * sin(g_roadTurn));
	while (mPosY < SCREEN_HEIGHT){
		
		// prepare to render
		mScale.w = (int)mWidthf;
		mPosX = (int)mPosXf;
		
		// render
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
