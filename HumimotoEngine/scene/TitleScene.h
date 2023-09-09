#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Cube.h"

#include "../../Class/Models/Block.h"
#include "../../Class/Effect/EffectManager.h"
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	int textureNum_;
	Input* input_;
	Vector3 pos_;

	Block* block_;
	// 処理の軽いBlock(背面カリング)
	Cube* cube_;
	EffectManager effectManager_;

	// worldTransformとviewProjectionの動作チェック用
	WorldTransform worldTransform_;
	WorldTransform cubeWorldTransform_;
	ViewProjection viewProjection_;
};