#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sprite.h"
#include "../object/Sphere.h"

#include "../../Class/Models/Block.h"
#include "../../Class/Effect/EffectManager.h"

class GameManager;

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
private:
	Sprite* sprite_;
	Sphere* sphere_;
	int textureNum_;
	Input* input_;
	Vector3 pos_;

	Block* block_;
	Block* parentBlock_;
	EffectManager effectManager_;
};