#pragma once
#include "../IEffect.h"

// includeが必要なファイルは可能な限りcppでincludeすること（循環参照防止）

// エフェクトの名前をクラス名に
class EffectName
	: public IEffect {
public:	// メンバ関数
	EffectName(/* 必要な引数を自由に定義可能 */);
	~EffectName();

	// エフェクトの処理が終わったら必ずisEndをtrueに（変数はIEffect.hで定義済み）
	// Update、Drawは最初にisEndがtrueならばreturnすること

	void Initialize() override;	// 初期化
	void Update() override;	// 更新
	void Draw() override;	// 描画

private: // メンバ変数
	// 必要な変数はここで定義
};