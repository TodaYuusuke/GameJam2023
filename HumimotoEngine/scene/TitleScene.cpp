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
	parentBlock_ = new Block();
	parentBlock_->Initialize();

	parentBlock_->parent_ = block_->parent_;
	pos_ = { 0,0,30 };
}

void TitleScene::Update() {
	ImGui::Begin("father");
	ImGui::DragFloat3("pos", &pos_.x, 0.1f);
	ImGui::End();
}

void TitleScene::Draw() {
	//sprite_->Draw(pos_, textureNum_);
	//sphere_->Draw();
	block_->Draw(pos_, {1,1,1},{0,0,0},BLOCK);
	parentBlock_->Draw({ 0,0,-30 }, { 1,1,1 }, { 0,0,0 }, UVCHEKER);
}

void TitleScene::Finalize() {
	delete sprite_;
	delete sphere_;
	delete block_;
}