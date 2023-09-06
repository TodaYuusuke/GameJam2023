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
	pos_ = { 0,0,30 };
}

void TitleScene::Update() {

}

void TitleScene::Draw() {
	block_->Draw(pos_, {1,1,1},{0,0,0},BLOCK);
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
	delete block_;
}