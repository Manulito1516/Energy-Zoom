#include "TitleScene.h"
#include "GameScene.h"
#include "../setup.h"
#include "../ME/ME_Texture.h"
#include "../ME/ME_Scene.h"

TitleScene* TitleScene::get(){
	//Get static instance
	return &sTitleScene;
}

void TitleScene::enter(){

	//Load background 
	mTitleTexture.load("assets/titlebg.png", 512, 256); // actually 320x240 but limitations
}

void TitleScene::exit(){
	mTitleTexture.free();

	//return true;
}

void TitleScene::takeInput(SDL_Event &event){
	//If the user pressed enter
	if((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_RETURN)){
		//Move onto title state
		setNextScene(GameScene::get());
	}
}

void TitleScene::update(){

}

void TitleScene::render(){
	//Show the background
	mTitleTexture.render(0, 0);
}

//Declare static instance
TitleScene TitleScene::sTitleScene;

TitleScene::TitleScene(){
	//No public instantiation
}
