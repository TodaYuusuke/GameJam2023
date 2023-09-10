#include "Map.h"
#include "../../HumimotoEngine/Manager/ImGuiManager.h"
#include "../Models/Player.h"

void Map::Initialize() {
	// ブロックをクリア
	blocks_.clear();

	// 地面のブロックを生成
	AddGroundBlock();
	// マップチップを参照し、必要なブロックを生成
	for (int x = 0; x < GetXSize(); x++) {
		for (int y = 0; y < GetYSize(); y++) {
			for (int z = 0; z < GetZSize(); z++) {
				switch (mapChip_[x][y][z])
				{
					case BlockTypeID::Air:
					case BlockTypeID::StartPosition:
					case BlockTypeID::OutOfArea:
						break;
					case BlockTypeID::Wall:
					default:
						Block* newBlock = new Block();
						newBlock->Initialize();
						newBlock->transform.translate = GetWorldPosition(x, y, z);
						newBlock->textureNum = BLOCK;
						blocks_.push_back(newBlock);
						break;
				}
			}
		}
	}

	// スケールを調整
}
void Map::Update() {
	ImGui::Begin("Map Test");
	ImGui::DragFloat3("block t", &groundBlocks_[0]->transform.translate.x, 0.1f);
	ImGui::DragFloat3("block r", &groundBlocks_[0]->transform.rotate.x, 0.1f);
	ImGui::DragFloat3("block s", &groundBlocks_[0]->transform.scale.x, 0.1f);
	ImGui::End();
}
void Map::Draw() {
	for (Block* block : groundBlocks_) {
		block->Draw();
	}
	for (Block* block : blocks_) {
		block->Draw();
	}
}

bool Map::CheckClear() {
	for (int x = 0; x < GetXSize(); x++) {
		for (int y = 0; y < GetYSize(); y++) {
			for (int z = 0; z < GetZSize(); z++) {
				switch (mapChip_[x][y][z])
				{
					case BlockTypeID::Air:
					case BlockTypeID::Wall:
					case BlockTypeID::StartPosition:
					case BlockTypeID::OutOfArea:
						break;
					default:
						return false;
				}
			}
		}
	}

	return true;
}

void Map::ClearMapChip() {
	for (int x = 0; x < GetXSize(); x++) {
		for (int y = 0; y < GetYSize(); y++) {
			for (int z = 0; z < GetZSize(); z++) {
				mapChip_[x][y][z] = BlockTypeID::OutOfArea;	// エリア外で初期化
			}
		}
	}
}
void Map::SetMapChip(Vector3 position, BlockTypeID id) {
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);
	int z = static_cast<int>(position.z);

	// x、y、zが現在のmapChip_のサイズを超えている場合、拡張する
	while (x >= mapChip_.size()) {
		mapChip_.push_back(std::vector<std::vector<BlockTypeID>>()); // 新しいx層を追加
	}

	while (y >= mapChip_[x].size()) {
		mapChip_[x].push_back(std::vector<BlockTypeID>()); // 新しいy層を追加
	}

	while (z >= mapChip_[x][y].size()) {
		mapChip_[x][y].push_back(BlockTypeID::OutOfArea); // 新しいz層を追加
	}

	// 指定した位置にidを設定
	mapChip_[x][y][z] = id;
}
void Map::ReverseZ() {
	for (std::vector<std::vector<BlockTypeID>>& xRow : mapChip_) {
		for (std::vector<BlockTypeID>& yRow : xRow) {
			std::reverse(yRow.begin(), yRow.end()); // Z軸を逆順に並び替え
		}
	}
}


Vector3 Map::GetWorldPosition(int x, int y, int z) {
	return { x * kBlockSize_ * kBlockScale_, y * kBlockSize_ * kBlockScale_, z * kBlockSize_ * kBlockScale_ };
}
void Map::AddGroundBlock() {
	for (int x = 0; x < GetXSize(); x++) {
		for (int z = 0; z < GetZSize(); z++) {
			Block* newBlock = new Block();
			newBlock->Initialize();
			newBlock->transform.translate = GetWorldPosition(x, -1, z);
			newBlock->textureNum = BLOCK;
			groundBlocks_.push_back(newBlock);
		}
	}
}

Vector3 Map::GetMapCenterPosition() {
	Vector3 centerPos = GetWorldPosition((int)GetXSize(), (int)GetYSize(), (int)GetZSize());
	centerPos.x /= 2.0f;
	centerPos.x -= kBlockSize_ / 2.0f;
	centerPos.y = 0.0f;
	centerPos.z /= 2.0f;
	centerPos.z -= kBlockSize_ / 2.0f;
	return centerPos;
}

Vector3 Map::IsCollisionVector3(AABB aabb) {
	// 最終的に返すベクトル
	Vector3 result = { 0.0f,0.0f,0.0f };
	float limitX = (GetXSize() * (kBlockSize_ * kBlockScale_)) - 1.0f;
	float limitZ = (GetZSize() * (kBlockSize_ * kBlockScale_)) - 1.0f;

	// 場外に出ていないかチェック
	if (aabb.min.x < -1.0f) {
		result.x -= 1.0f + aabb.min.x;
	}
	else if(aabb.max.x > limitX){
		result.x += limitX - aabb.max.x;
	}
	if (aabb.min.z < -1.0f) {
		result.z -= 1.0f + aabb.min.z;
	}
	else if (aabb.max.z > limitX) {
		result.z += limitX - aabb.max.z;
	}


	// 全ブロックとの当たり判定を取る
	for (Block* block : blocks_) {
		// ブロックからAABBを計算する
		AABB collision;
		Vector3 blockSize = { (kBlockSize_ / 2.0f) * kBlockScale_ ,(kBlockSize_ / 2.0f) * kBlockScale_ , (kBlockSize_ / 2.0f) * kBlockScale_ };
		collision.min = block->transform.translate - blockSize;
		collision.max = block->transform.translate + blockSize;
		// 当たり判定をチェック
		Vector3 fixVector = aabb.IsCollisionVector3(collision);
		// 計算するAABBを修正してから、当たり判定に戻る
		aabb.min += fixVector;
		aabb.max += fixVector;
		result += fixVector;
	}

	return result;
}