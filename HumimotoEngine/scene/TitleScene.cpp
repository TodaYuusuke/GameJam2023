#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	sprite_ = new Sprite();
	sprite_->Initialize({0,0,0}, {1280,720,0});
	titleSprite_ = new Sprite();
	titleSprite_->Initialize({ 0,0,0 }, { 640,360,0 });
	textureNum_ = UVCHEKER;
	input_ = Input::GetInstance();
	block_ = new Block();
	block_->Initialize();
	block_->textureNum = BLOCK;
}

void TitleScene::Update() {
	XINPUT_STATE joyState{};
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
	}


}

void TitleScene::Draw() {
	block_->Draw();
	sprite_->Draw({0,0,0}, UVCHEKER);
	titleSprite_->Draw({ 320,100,0 }, BLOCK);
}

void TitleScene::Finalize() {
	delete sprite_;
	delete titleSprite_;
	delete block_;
}