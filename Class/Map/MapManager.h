#pragma once
#include "Map.h"


/// <summary>
/// マップの情報を持つクラス
/// </summary>
class MapManager {
public: // メンバ関数
	void Initialize();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画

private: // メンバ定数
	// マップデータを示したファイルパス
	const char* filePath_ = "../Resources/Const/MapData.csv";
	const static int kMapChipMaxSize_ = 8;	// マップデータの数

private: // メンバ変数
	
	// 全マップデータ
	Map map_[kMapChipMaxSize_];

private: // プライベートな関数
	// ファイルから全マップのデータを読み込む関数
	void LoadMapData();
};