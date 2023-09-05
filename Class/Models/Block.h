#pragma once
#include "../../HumimotoEngine/Manager/ObjManager.h"

class Block {
public: // メンバ関数
	void Initialize();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画

private: // メンバ変数

	// モデルのポインタ
	ModelData* modelData_;
	// ワールドトランスフォーム
	//WorldTransform worldTransform_;
};