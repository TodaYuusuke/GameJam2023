#include "GameClear.h"
#include "../Manager/GameManager.h"

void GameClear::Initialize() {
	// スプライトの生成

	// 背景
	gameclear_ = new Sprite();
	gameclear_->Initialize({ 0,0,1 }, { 1280,720,1 });
	sceneNum = GAMECLEAR_SCENE;
	input_ = Input::GetInstance();
}

void GameClear::Update() {
	// Aボタンを押すとゲームシーン切り替え
	XINPUT_STATE joyState{};
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (!preA && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		sceneNum = TITLE_SCENE;
	}
	preA = joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
}

void GameClear::Draw() {
	gameclear_->Draw({ 0,0,0 }, CLEARTITLE);

}

void GameClear::Finalize() {
	delete gameclear_;

}