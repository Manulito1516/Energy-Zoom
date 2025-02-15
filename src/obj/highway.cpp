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
#include "../scenes/GameScene.h"
using namespace std;

// arreglen los header carajo
const float Highway::HIGHWAY_VEL = 0.01;
const float Highway::HIGHWAY_MAX_VEL = 0.85;

Highway::Highway(){}

void Highway::create(GameScene* parent){
	mParent = parent;
	
	mPosX = SCREEN_WIDTH / 2; // Centro
	mPosY = SCREEN_HEIGHT / 2; // de la mitad para abajo
	
	mStartPosY = SCREEN_HEIGHT / 3;

	// Positions and velocity
	//mParent->s_posZf = 0.0;
	mParent->s_vel = 0.0;
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
	mParent->s_roadX = -26; // this isn't working
	
	mParent->s_roadAngle = mParent->s_roadX;
	
	mPosXf = mPosX;
	mWidthf = mScale.w;
	
	mClipYf = 0.5; // posicion del clip.y basada en la velocidad del jugador
	
	mParent->s_track.seekg(0, ifstream::beg);
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
	mParent->s_roadX += mVelXf * (mParent->s_vel/HIGHWAY_MAX_VEL);
	
	//Road turns
	if (mParent->s_vel > 0) {
		mParent->s_roadTurn += mTurnVel;
	}
	
	// Acceleration and Z axis things
	if (mParent->s_vel < mMaxVelocity){
		mAcceleration = mThrottle / 2; // is this temporary?
		mParent->s_vel += mAcceleration;
	}
	
	// "Friction", "air resistance", yeah, you get it
	if (mParent->s_vel > 0){
		mParent->s_vel -= HIGHWAY_VEL / 4 * (1+mBrake);
	} else if (mParent->s_vel < 0){
		mParent->s_vel += HIGHWAY_VEL / 4 * (1+mBrake);
	}
	
	// Clip min values
	if (mParent->s_vel < 0.001 || mParent->s_vel > 3){ // realistically speaking, 3 is too fast
		mParent->s_vel = 0;
	}
	
	// Position (DON'T MODIFY DIRECTLY)
	mClipYf -= mParent->s_vel; // controls the texture side (note that this is negative)
	mParent->s_posZf += mParent->s_vel; // controls the position on circuit
	
	// Moves the car out if it goes straight during a turn
	if (mParent->s_vel > HIGHWAY_MAX_VEL / 4){
		mParent->s_roadX += mParent->s_roadTurn * (mParent->s_vel + mThrottle * 80) * 0.9; // higher number means lower grip
	}
	
	// Hitbox
	mHitbox.x = (mParent->s_roadX + 24) *9 + SCREEN_WIDTH * sin(mParent->s_roadTurn) * 2.5 - (mHitbox.w - SCREEN_WIDTH) / 2;//- SCREEN_WIDTH / 2;
	
	// *** COLLISIONS ***
	// road
	if (mParent->s_onRoad){
		//on road
		mMaxVelocity = HIGHWAY_MAX_VEL; 
	} else {
		//out of the road
		//mParent->s_vel -= HIGHWAY_VEL * 4;
		mMaxVelocity = HIGHWAY_MAX_VEL / 4;
	}
}

void Highway::readRoad(){
	if (mParent->s_posZf > mTriggerPos){ // after trigger position
		if (mTriggerPos == 0){
			mParent->s_track.seekg(0, ifstream::beg);
			loadTrigger();
		}
		
		// first check positivity, then if it's not enough...
		if (mTriggerTurnTarget >= 0 && mParent->s_roadTurn < mTriggerTurnTarget) {
			mTurnVel = mTriggerTurnSpeed * (1+mParent->s_vel); // turns right
			
		// check negativity, if it's not enough...
		} else if (mTriggerTurnTarget <= 0 && mParent->s_roadTurn > mTriggerTurnTarget) {
			mTurnVel = -mTriggerTurnSpeed * (1+mParent->s_vel); // turns left
		} else {
			mTurnVel = 0;
		}
		
		// return to straight
		if (mTriggerTurnTarget == 0 && mParent->s_roadTurn > -0.01 && mParent->s_roadTurn < 0.01) {
			mTurnVel = 0;
			mParent->s_roadTurn = 0;
		}
		
		// same case just after :)
		if ((mTriggerTurnTarget > 0 && mParent->s_roadTurn > mTriggerTurnTarget)
		or (mTriggerTurnTarget == 0 && mParent->s_roadTurn == 0 && mTriggerPos != 0)
		or (mTriggerTurnTarget < 0 && mParent->s_roadTurn < mTriggerTurnTarget)) {
			mTriggerNumber++;
			loadTrigger();
		} 
	}
	
	if (mParent->s_posZf > mTriggerLoopPos){
		// temp
		mTriggerNumber = 1;
		mParent->s_posZf = 0;

		mParent->reloadTrack();
		loadTrigger();
	}
}

void Highway::loadTrigger(){
	if (!mParent->s_track.is_open()){
		return;
	}
	
	// make temp strings
	string sTriggerPos;
	string sTriggerTurnTarget;
	string sTriggerTurnPos;
	
	// get the data from file mParent->s_track in strings
	getline(mParent->s_track, sTriggerPos, ',');
	getline(mParent->s_track, sTriggerTurnTarget, ',');
	getline(mParent->s_track, sTriggerTurnPos, ';');
	
	// string to int, string to double (float)
	mTriggerPos = atoi(sTriggerPos.c_str());
	mTriggerTurnTarget = (float)atof(sTriggerTurnTarget.c_str());
	mTriggerTurnSpeed = (float)atof(sTriggerTurnPos.c_str());
	
	if (mTriggerPos == 0) {
		mTriggerLoopPos = (int)mParent->s_posZf;
	}
}

void Highway::render(ME_Texture* texture){
	mWidthf = 2;
	mPosXf = (SCREEN_WIDTH / 2 - mWidthf) + ((SCREEN_WIDTH) * sin(mParent->s_roadTurn));
	
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
	
	// Como mParent->s_roadAngle va a ir cambiando dentro del loop, lo "reinicio" antes de entrar
	mParent->s_roadAngle = mParent->s_roadX - ((SCREEN_WIDTH / 4) * sin(degToRad(mParent->s_roadTurn)));
	while (mPosY < SCREEN_HEIGHT){
		
		// prepare to render
		mScale.w = (int)mWidthf;
		mPosX = (int)mPosXf;
		
		//***************** RENDER *****************
		texture->render(mPosX, mPosY, &mScale, &mClip);
		//***************** RENDER *****************
		
		// calculate road angle
		mWidthf += 4* cos(degToRad(mParent->s_roadAngle));
		mPosXf += 4* sin(degToRad(mParent->s_roadAngle));
		
		// roadTurn
		mParent->s_roadAngle += mParent->s_roadTurn;
		
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
