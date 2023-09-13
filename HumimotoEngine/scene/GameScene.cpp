#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	camera_ = DebugCamera::GetInstance();
	sceneNum = GAME_SCENE;
	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize();

	Retry();
}

void GameScene::Update() {
	// クリアチェック
	if (map_->CheckClear()) {
		loadStageNum_++;
		if (loadStageNum_ >= 9) {
			sceneNum = GAMECLEAR_SCENE;
		}
		Retry();
	}

	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// ホットリロード
		if (!preX && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			Retry();
		}
		preX = joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	}

	ImGui::Begin("GameScene Debug");
	ImGui::SliderInt("loadStageLevel", &loadStageNum_, 0, 8);
	ImGui::End();

	player_.Update();
	map_->Update();
}

void GameScene::Draw() {
	skydome_->Draw();
	map_->Draw();
	player_.Draw();
}

void GameScene::Finalize() {
}

void GameScene::Retry() {
	MapManager::GetInstance()->Initialize();
	MapManager::GetInstance()->SetCurrentMap(loadStageNum_);

	map_ = MapManager::GetInstance()->GetCurrentMap();
	player_.Initialize(MapManager::GetInstance()->GetCurrentMap()->GetPlayerStartPosition());

	camera_->Initialize();
	camera_->SetRotationCenterPosition();
}