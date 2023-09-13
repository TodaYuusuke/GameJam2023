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
	Sprite* background_;
	Sprite* titleName_;
	Sprite* guideA_Button_;
	Sprite* guidePress_;

	Input* input_;
	EffectManager effectManager_;

	// ƒ^ƒCƒgƒ‹‚ğ“®‚©‚·‚½‚ß”¼”
	Vector2 titleNamePos_;
	float theta_;
};