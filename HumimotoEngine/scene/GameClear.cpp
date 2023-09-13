#include "GameClear.h"
#include "../Manager/GameManager.h"

void GameClear::Initialize() {
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

}

void GameClear::Finalize() {

}