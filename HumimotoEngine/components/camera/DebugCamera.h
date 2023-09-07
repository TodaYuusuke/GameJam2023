#pragma once
#include "../../math/Vector3.h"
#include "../../math/Matrix4x4.h"

class DebugCamera
{
public:
	static DebugCamera* GetInstance();

	void Initialize();

	void Update();

	void SetRotationCenterPosition();

	Matrix4x4 GetViewMatrix() { return viewMatrix_; }

private:
	// 回転中心座標
	Vector3 rotationCenterPosition_;
	// 回転のオフセット
	Vector3 offset_;
	// 中心からの回転
	Vector3 offsetRotation_;

	// x,y,z軸周りのローカル回転角
	Vector3 rotation_;
	// 累積回転行列
	Matrix4x4 matRot_;

	// ローカル座標
	Vector3 translation_;
	// ワールド座標
	Matrix4x4 worldTransform_;
	// ビュー行列
	Matrix4x4 viewMatrix_;
	// 射影行列
	Matrix4x4 orthographicMatrix_;
};

