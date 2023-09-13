#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sprite.h"

#include "../../Class/Models/Block.h"
#include "../../Class/Effect/EffectManager.h"

class GameManager;
class GameClear : public IScene
{
public:
	// 初期化
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
private:
	Sprite* gameclear_;

	Input* input_;
	EffectManager effectManager_;
	bool preA = false;
	float theta_;
};

