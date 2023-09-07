#include "GameScene.h"
#include "../Manager/GameManager.h"

void GameScene::Initialize() {
	camera_ = DebugCamera::GetInstance();
	camera_->Initialize();
	camera_->SetRotationCenterPosition();

	sceneNum = GAME_SCENE;

	map_ = MapManager::GetInstance()->GetCurrentMap();
}

void GameScene::Update() {
	// ホットリロード
	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		MapManager::GetInstance()->Initialize();
		MapManager::GetInstance()->SetCurrentMap(0);
		Initialize();
	}




	//camera_->Update();
	map_->Update();
}

void GameScene::Draw() {
	map_->Draw();
}

void GameScene::Finalize() {

}