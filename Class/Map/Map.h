#pragma once
#include "../../HumimotoEngine/math/Vector3.h"
#include "../Models/Block.h"
#include "../Utility/AABB.h"

class Player;

// *** マップID *** //
// 0 ... 無し（空気）
// 99 ... 壁（動かせない、消滅しない）
// 100 ... プレイヤー開始地点
// -1 ... プレイヤー

// 1～98 ... ブロック（同じ番号のブロックは結合する）
enum class BlockTypeID : int {
	Air = 0,
	Wall = 99,
	StartPosition = 100,
	OutOfArea = -1,
	Player = -2,
};

struct MapChip {
	// マップチップ
	BlockTypeID type = BlockTypeID::OutOfArea;
	// ブロックのポインタを持つ
	Block* blockPtr = nullptr;
};

class Map {
public: // メンバ関数
	void Initialize();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画
	
	// クリアか判定する
	bool CheckClear();

	// マップIDをクリア
	void ClearMapChip();
	// 指定した座標にマップIDをセットする
	void SetMapChip(Vector3 position, BlockTypeID id);
	// 最後にZ軸を反転させる用
	void ReverseZ();
	// 指定した座標のマップIDをゲットする
	BlockTypeID GetMapChip(Vector3 position) { return mapChip_[(int)position.x][(int)position.y][(int)position.z].type; }

	// マップのノルマをセットする
	void SetQuota(int quota1, int quota2, int quota3) { 
		quota[0] = quota1;
		quota[1] = quota2;
		quota[2] = quota3;
	}

	// マップのサイズを受け取る
	size_t GetXSize() { return mapChip_.size(); }
	size_t GetYSize() { return (GetXSize() > 0) ? mapChip_[0].size() : 0; }
	size_t GetZSize() { return (GetXSize() > 0 && GetYSize() > 0) ? mapChip_[0][0].size() : 0; }

	// マップの中心座標を受け取る
	Vector3 GetMapCenterPosition();

	// マップの当たり判定を取る
	Vector3 IsCollisionVector3(AABB aabb);
	// ブロックをつかめるか判定を取る
	bool GrabBlock(Player* player);
	// ブロックを手放す際の処理
	void ReleaseBlock();

private: // メンバ定数
	float kBlockSize_ = 2.0f;	// ブロック一つのサイズ
	float kBlockScale_ = 1.0f;	// ブロックの共通スケール

	int kBlockMoveCoolTime_ = 30;	// ブロックを動かす際のクールタイム

private: // メンバ変数
	
	// マップチップ
	std::vector<std::vector<std::vector<MapChip>>> mapChip_;
	
	// 地面の描画用ブロックのデータ
	std::vector<Block*> groundBlocks_;

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// プレイヤーのつかんだブロックID
	int grabbedBlockID = -1;
	// ブロックを動かすクールタイム
	int blockMoveCoolTime = 0;

	// マップのノルマ
	int quota[3];

private: // プライベートな関数
	// xyzからワールド座標を求める
	Vector3 GetWorldPosition(int x, int y, int z);
	// ワールド座標から近いxyzを求める
	Vector3 GetMapChipPosition(Vector3 worldPosition);
	// 地面の描画要のブロックを追加する
	void AddGroundBlock();

	// マップチップ上での移動処理
	bool MoveOnMapChip(Vector3 destination);
};