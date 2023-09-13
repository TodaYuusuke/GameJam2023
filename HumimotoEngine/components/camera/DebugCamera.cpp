#include "DebugCamera.h"
#include "../../components/Input.h"
#include "../../Manager/ImGuiManager.h"
#include "../../../Class/Map/MapManager.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;

	return &instance;
}

void DebugCamera::Initialize() {
	// x,y,z軸周りのローカル回転角
	rotation_ = { 0,0,0 };
	// ローカル座標
	translation_ = { 0,0,-50 };

	// 回転中心座標
	// オフセット
	offset_ = { 0.0f, 0.0f, -30.0f };
	offsetRotation_ = { 0.600f,0.500f,0.0f };
}

void DebugCamera::Update() {

	// コントローラーでの操作
	InputController();
	// キーボードでの操作
	InputKeyboard();


	// 回転の数値が増えすぎないように修正
	if (offsetRotation_.x > 6.28f)
		offsetRotation_.x -= 6.28f;
	if (offsetRotation_.x < 0.0f)
		offsetRotation_.x += 6.28f;
	if (offsetRotation_.y > 6.28f)
		offsetRotation_.y -= 6.28f;
	if (offsetRotation_.y < 0.0f)
		offsetRotation_.y += 6.28f;
	if (offsetRotation_.z > 6.28f)
		offsetRotation_.z -= 6.28f;
	if (offsetRotation_.z < 0.0f)
		offsetRotation_.z += 6.28f;

	// カメラ制限
	if (kCameraZLimitMin < offset_.z) {
		offset_.z = kCameraZLimitMin;
	}
	else if (kCameraZLimitMax > offset_.z) {
		offset_.z = kCameraZLimitMax;
	}

	if (kCameraRotationXLimitMin > offsetRotation_.x) {
		offsetRotation_.x = kCameraRotationXLimitMin;
	}
	else if (kCameraRotationXLimitMax < offsetRotation_.x) {
		offsetRotation_.x = kCameraRotationXLimitMax;
	}

	Vector3 offset = TransformNormal(offset_, MakeRotateMatrix(offsetRotation_));

	// 座標をオフセット分ずらす
	translation_ = Add(rotationCenterPosition_, offset);
	rotation_ = offsetRotation_;

	worldTransform_ = MakeAffineMatrix({ 1,1,1 }, rotation_, translation_);
	viewMatrix_ = Inverse(worldTransform_);
}

void DebugCamera::SetRotationCenterPosition() {
	rotationCenterPosition_ = MapManager::GetInstance()->GetCurrentMap()->GetMapCenterPosition();
}

void DebugCamera::InputController() {
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// デッドゾーンの設定
		SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRX);
		SHORT leftThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbRY);

		// カメラ回転
		Vector3 turn = {
			-(float)leftThumbY / SHRT_MAX,
			(float)leftThumbX / SHRT_MAX,
			0.0f };
		offsetRotation_.x += turn.x * kCameraTurnSpeed;
		offsetRotation_.y += turn.y * kCameraTurnSpeed;
		offsetRotation_.z += turn.z * kCameraTurnSpeed;

		// カメラズーム
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
			offset_.z += kCameraZoomSpeed;
		}
		else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
			offset_.z -= kCameraZoomSpeed;
		}
	}
}
void DebugCamera::InputKeyboard() {

}