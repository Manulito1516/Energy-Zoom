#ifndef ME_GAMESCENE_H
#define ME_GAMESCENE_H

#include <iosfwd>
#include "../ME/ME_Scene.h"
#include "../ME/ME_Texture.h"
#include "../obj/circle.h"
#include "../obj/car.h"
#include "../obj/highway.h"
#include "../obj/bg.h"
#include "../obj/grass.h"
#include "../obj/object3D.h"
using namespace std;

class Car;
class Highway;
class Obj3D;

class GameScene : public ME_Scene {
	public:
		//Static accessor
		static GameScene* get();

		//Transitions
		void enter();
		void exit();

		//Main loop functions
		void takeInput(SDL_Event &event);
		void update();
		void render();
		
		void reloadTrack();
		
		// scene local vars (like globals but only for this scene)
		float s_posZf;
		float s_roadTurn;
		float s_roadX;
		float s_roadAngle;
		float s_vel;
		bool s_onRoad;
		ifstream s_track;
	private:
		//Static instance
		static GameScene sGameScene;
		
		//Scene objects
		Circle o_circle;
		Highway* o_highway;
		Car* o_car;
		Obj3D* o_obj3d;
		BG o_bg;
		Grass o_grass;
		
		
		//textures
		ME_Texture mCircleTexture;
		ME_Texture mBGTexture;
		ME_Texture mCarTexture;
		ME_Texture mGrassTexture;
		ME_Texture mHighwayTexture;

		//Private constructor
		GameScene();

};

#endif
