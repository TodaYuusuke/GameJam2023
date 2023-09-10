#pragma once
#include "IScene.h"
#include "../components/camera/DebugCamera.h"
#include "../../Class/Map/MapManager.h"
#include "../../Class/Models/Player.h"

class GameManager;
class GameScene : public IScene
{
public:
	// 初期化
	void Initialize()override;

	// 更新処理
	void Update()override;

	// 描画
	void Draw()override;

	void Finalize()override;
private:
	// カメラのポインタ
	DebugCamera* camera_ = nullptr;

	// マップデータ
	Map* map_ = nullptr;
	// プレイヤー
	Player player_;

	// デバッグ用の変数
	int loadStageNum_ = 0;	// 読み込むステージの番号
};

