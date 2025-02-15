#include "GameScene.h"
#include <iosfwd>
#include "../setup.h"
#include "../ME/ME_Texture.h"
#include "../obj/circle.h"
#include "../obj/car.h"
#include "../obj/highway.h"
#include "../obj/bg.h"
#include "../obj/grass.h"
#include "../obj/object3D.h"

GameScene* GameScene::get(){
	//Get static instance
	return &sGameScene;
}

void GameScene::enter(){
	reloadTrack();
	// scene locals
	s_posZf = 0;
	s_roadTurn = 0;
	s_roadX = 0; // perspective of position and turn
	s_roadAngle = 0;
	s_vel = 0;
	s_onRoad = true;
	
	//Scene objects
	Circle o_circle;
	BG o_bg;
	Grass o_grass;
	
	o_highway = new Highway;
	o_car = new Car;
	o_obj3d = new Obj3D;
	o_highway->create(this);
	o_car->create(this);
	o_obj3d->create(this);
	
	//textures
	mCircleTexture.load("assets/circle.png", o_circle.CIRCLE_WIDTH, o_circle.CIRCLE_HEIGHT);
	mBGTexture.load("assets/bg/bg.png", o_bg.BG_WIDTH, o_bg.BG_HEIGHT);
	mGrassTexture.load("assets/bg/grass.png", o_grass.GRASS_TEX_W, o_grass.GRASS_TEX_H);
	mCarTexture.load("assets/sprites/carsheet.png", o_car->CAR_WIDTH, o_car->CAR_HEIGHT);
	mHighwayTexture.load("assets/sprites/highway.png", o_highway->HIGHWAY_TEX_W, o_highway->HIGHWAY_TEX_H);
	
	
}

void GameScene::exit(){
	s_track.close();
	
	mCircleTexture.free();
	mBGTexture.free();
	mGrassTexture.free();
	mCarTexture.free();
	mHighwayTexture.free();
}

void GameScene::takeInput(SDL_Event &event){
	o_circle.takeInput(event);
	o_highway->takeInput(event);
	o_car->takeInput(event);
}

void GameScene::reloadTrack(){
	if (s_track.is_open()){
		s_track.close();
		//s_track.clear();
		//s_track.seekg(0);
	}
	
	s_track.open("tracks/track1");
}

void GameScene::update(){
	// TODO: change this to mOnRoad
	s_onRoad = checkCollision(o_highway->get_hitbox(), o_car->get_hitbox());
	
	o_circle.update();
	o_highway->update();
	o_highway->readRoad();
	o_car->update();
}

void GameScene::render(){
	// render background
	o_bg.render(&mBGTexture);
	// objects
	o_grass.render(&mGrassTexture, o_highway->get_clipYf());
	o_highway->render(&mHighwayTexture);
	o_car->render(&mCarTexture);
	o_circle.render(&mCircleTexture);
	o_obj3d->render(&mCircleTexture);
	
	// DEBUG
	int numbar = 10;
	std::string debugTexts[numbar] = { // this might be a problem
		"s_posZ: " + std::to_string(s_posZf),
		"s_vel: " + std::to_string(s_vel),
		"highway.mAccel: " + std::to_string(o_highway->get_accel()),
		"s_roadTurn: " + std::to_string(s_roadTurn),
		"mNextTrigger: " + std::to_string(o_highway->get_nextTrigger()),
		"mTriggerPos: " + std::to_string(o_highway->get_triggerPos()),
		"s_onRoad: " + std::to_string(s_onRoad),
		"cardir: " + std::to_string(o_car->get_dir()),
		"s_roadX " + std::to_string(s_roadX),
		"mScaleFactor " + std::to_string(o_obj3d->get_scaleFactor())
	};
	
	for (int i = 0; i < numbar; i++){
		g_manusFont.renderText(0, 12 + i * 12, debugTexts[i]);
	}
	
}

//Declare static instance
GameScene GameScene::sGameScene;

GameScene::GameScene(){
	//No public instantiation
}
