#include "object3D.h"

#include <SDL2/SDL.h>
#include <string.h>
//#include <stdio.h>
#include <iosfwd>
#include <vector>
#include <cmath>

#include "../setup.h"

#include "../ME/ME_Texture.h"
#include "../scenes/GameScene.h"

// constructor
Obj3D::Obj3D(){}

void Obj3D::create(GameScene* parent){
	mParent = parent;
	
	mX = 0;
	mY = 0;
	mZ = 300; // position on track
	
	mXRender = 0; // used by render function, do not the mXRender
	mYRender = HORIZON; // used by render function, do not the mYRender
	mScaleFactor = 1;
	
	mScale.x = 0;
	mScale.y = 0;
	mScale.w = 32;
	mScale.h = 32;
	
	mClip.x = 0;
	mClip.y = 0;
	mClip.w = 32;
	mClip.h = 32;
}

void Obj3D::render(ME_Texture* texture){
	if (mParent->s_posZf >= mZ){
		return;
	}
	//mZ no debe ser 0
	mScaleFactor = mZ / (mZ + abs(mZ - mParent->s_posZf) * 40); // (mZ - mParent->s_posZf) = 0 cuando llega al trigger (no necesariamente a la altura del auto)
	if (mScaleFactor < 0.02 || mScaleFactor > 1){
		return;
	}
	
	// render
	mScale.w = 1 + WIDTH * mScaleFactor;
	mScale.h = 1 + HEIGHT * mScaleFactor;
	
	mYRender = HORIZON - 5 + (SCREEN_HEIGHT - HORIZON) * mScaleFactor;
							 		
	mXRender =
		(SCREEN_WIDTH / 2 - mScale.w / 2) 		// centro
		+ mX * mScaleFactor 					// offset
		
		+ (mYRender - HORIZON) * 4 * sin(degToRad(mParent->s_roadAngle + (mYRender - HORIZON) * mParent->s_roadTurn))
		+ SCREEN_WIDTH * sin(mParent->s_roadTurn) // no cambia nada
		
		//+ 4* (mYRender - SCREEN_HEIGHT / 3) * sin(degToRad(mParent->s_roadAngle));		// posicion x del auto
		
	;
	texture->render(mXRender, mYRender + mY, &mScale, &mClip);
}

void Obj3D::update(){
	//mX += mSpeedX;
	//mY += mSpeedY;
}
