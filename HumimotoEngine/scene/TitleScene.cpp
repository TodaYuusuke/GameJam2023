#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	sprite_ = new Sprite();
	sprite_->Initialize();
	sphere_ = new Sphere();
	sphere_->Initialize();
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	block_ = new Block();
	block_->Initialize();
}

void TitleScene::Update() {

}

void TitleScene::Draw() {
	//sprite_->Draw(pos_, textureNum_);
	//sphere_->Draw();
	block_->Draw({ 0,0,0 }, {1,1,1},{0,0,0},UVCHEKER);
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
	delete block_;
}