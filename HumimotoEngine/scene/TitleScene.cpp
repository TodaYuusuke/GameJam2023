#include "TitleScene.h"
#include "../Manager/GameManager.h"

void TitleScene::Initialize() {
	// タイトルの初期位置
	titleNamePos_ = { 128,100 };

	// スプライトの生成

	// 背景
	background_ = new Sprite();
	background_->Initialize({0,0,1}, {1280,720,1});
	// タイトル
	titleName_ = new Sprite();
	titleName_->Initialize({ 0,0,1 }, { 1024,256,1 });
	// AボタンのUI
	guideA_Button_ = new Sprite();
	guideA_Button_->Initialize({ 0,0,1 }, { 64,64,1 });
	// pressのテキスト
	guidePress_ = new Sprite();
	guidePress_->Initialize({ 0,0,1 }, { 128,64,1 });

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize();

	input_ = Input::GetInstance();
}

void TitleScene::Update() {
	// Aボタンを押すとゲームシーン切り替え
	XINPUT_STATE joyState{};
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE)) {
		sceneNum = GAME_SCENE;
	}

	// タイトルを上下に動かす
	theta_ += 1.0f / 90.0f * (float)M_PI;
	titleNamePos_.y += sinf(theta_) * 0.25f;
}

void TitleScene::Draw() {
	//background_->Draw({0,0,0}, BACKGROUND_GRAY);
	//titleName_->Draw({ titleNamePos_.x,titleNamePos_.y,0 }, TITLENAME);
	//guideA_Button_->Draw({ 704,500,0 }, A_BUTTON);
	//guidePress_->Draw({ 556,500,0 }, GUIDE_PRESS);
	skydome_->Draw();
}

void TitleScene::Finalize() {
	delete background_;
	delete titleName_;
	delete guideA_Button_;
	delete guidePress_;
	delete skydome_;
}