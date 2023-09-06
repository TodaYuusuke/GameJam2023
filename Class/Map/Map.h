#pragma once
#include "../../HumimotoEngine/math/Vector3.h"
#include "../Models/Block.h"

// *** マップID *** //
// 0 ... 無し（空気）
// 99 ... 地面や足場（動かせない、消滅しない）
// 100 ... プレイヤー開始地点
// -1 ... マップ領域外

// 1～98 ... ブロック（同じ番号のブロックは結合する）
enum class BlockTypeID : int {
	Air = 0,
	Ground = 99,
	StartPosition = 100,
	OutOfArea = -1,
};

class Map {
public: // メンバ関数
	void Initialize();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画
	
	// 指定した座標にマップIDをセットする
	void SetMapChip(Vector3 position, BlockTypeID id) { mapChip_[(int)position.x][(int)position.y][(int)position.z] = id; }
	// 指定した座標のマップIDをゲットする
	BlockTypeID GetMapChip(Vector3 position) { return mapChip_[(int)position.x][(int)position.y][(int)position.z]; }

private: // メンバ定数
	const static int kMapChipMaxSize_ = 16;	// マップの最大サイズ

private: // メンバ変数
	
	// マップチップ
	BlockTypeID mapChip_[kMapChipMaxSize_][kMapChipMaxSize_][kMapChipMaxSize_];

	// 描画用のブロックのデータ
	std::vector<Block> blocks;
};