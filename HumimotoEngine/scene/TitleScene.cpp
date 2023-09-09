#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	block_ = new Block();
	block_->Initialize();
	cube_ = new Cube();
	cube_->Initialize();

	worldTransform_.Initialize();
	cubeWorldTransform_.Initialize();
	viewProjection_.Initialize();

	cubeWorldTransform_.translation_ = { 4,0,0 };
}

void TitleScene::Update() {
	worldTransform_.UpdateMatrix();
	cubeWorldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	ImGui::Begin("Blocks Parameter");
	ImGui::DragFloat3("objBlock:translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("objBlock:rotation", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("objBlock:scale", &worldTransform_.scale_.x, 0.1f);
	ImGui::DragFloat3("meshBlock:translation", &cubeWorldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("meshBlock:rotation", &cubeWorldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("meshBlock:scale", &cubeWorldTransform_.scale_.x, 0.1f);
	ImGui::End();
}

void TitleScene::Draw() {
	block_->Draw(worldTransform_,viewProjection_,BLOCK);
	cube_->Draw(cubeWorldTransform_, viewProjection_, UVCHEKER);
}

void TitleScene::Finalize() {
	worldTransform_.constBuff_.ReleaseAndGetAddressOf();
	cubeWorldTransform_.constBuff_.ReleaseAndGetAddressOf();
	viewProjection_.constBuff_.ReleaseAndGetAddressOf();
	delete block_;
	delete cube_;
}