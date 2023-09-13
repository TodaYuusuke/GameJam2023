#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	sprite_ = new Sprite();
	sprite_->Initialize();
	sphere_ = new Sphere();
	sphere_->Initialize();
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	pos_ = { 0,0,30 };
}

void TitleScene::Update() {

}

void TitleScene::Draw() {
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
}