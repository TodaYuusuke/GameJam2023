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
void Map::Update() {}
void Map::Draw() {
	for (int x = 0; x < kMapChipMaxSize_; x++) {
		for (int y = 0; y < kMapChipMaxSize_; y++) {
			for (int z = 0; z < kMapChipMaxSize_; z++) {
				switch (mapChip_[x][y][z])
				{
					case BlockTypeID::Air:
						break;
					case BlockTypeID::Ground:
						break;
					case BlockTypeID::StartPosition:
						break;
					case BlockTypeID::OutOfArea:
						break;
					default:
						break;
				}
			}
		}
	}
}