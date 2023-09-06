#include "Map.h"

void Map::Initialize() {
	for (int x = 0; x < kMapChipMaxSize_; x++) {
		for (int y = 0; y < kMapChipMaxSize_; y++) {
			for (int z = 0; z < kMapChipMaxSize_; z++) {
				mapChip_[x][y][z] = BlockTypeID::OutOfArea;	// エリア外で初期化
			}
		}
	}
}
void Map::Update() {
	for (int x = 0; x < kMapChipMaxSize_; x++) {
		for (int y = 0; y < kMapChipMaxSize_; y++) {
			for (int z = 0; z < kMapChipMaxSize_; z++) {
				mapChip_[x][y][z] = BlockTypeID::OutOfArea;	// エリア外で初期化
			}
		}
	}
}
void Map::Draw() {

}