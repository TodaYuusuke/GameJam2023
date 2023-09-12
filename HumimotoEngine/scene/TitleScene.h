#pragma once
#include "IScene.h"
#include "../components/Input.h"
#include "../object/Sprite.h"

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
	Sprite* titleSprite_;
	//Sphere* sphere_;
	int textureNum_;
	Input* input_;

	Block* block_;
	EffectManager effectManager_;
};