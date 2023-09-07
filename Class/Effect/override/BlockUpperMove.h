#pragma once
#include "../IEffect.h"

// includeが必要なファイルは可能な限りcppでincludeすること（循環参照防止）

// エフェクトの名前をクラス名に
class BlockUpperMove
	: public IEffect {
public:	// メンバ関数
	BlockUpperMove(/* 必要な引数を自由に定義可能 */);
	~BlockUpperMove();

	// エフェクトの処理が終わったら必ずisEndをtrueに（変数はIEffect.hで定義済み）
	// Update、Drawは最初にisEndがtrueならばreturnすること

	void Initialize() override;	// 初期化
	void Update() override;	// 更新
	void Draw() override;	// 描画

private: // メンバ定数
	// エフェクトの詳細を調整するための定をここで定義

private: // メンバ変数

};