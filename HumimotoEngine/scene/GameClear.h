#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sprite.h"

class GameManager;
class GameClear : public IScene
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
	Input* input_;
	bool preA = false;
};

