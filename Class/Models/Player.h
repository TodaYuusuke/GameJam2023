#pragma once
#include <d3d12.h>
#include "../../HumimotoEngine/Manager/ImGuiManager.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../../HumimotoEngine/utility/GlobalVariables.h"
#include "../../HumimotoEngine/components/camera/Camera.h"
#include "../../HumimotoEngine/Manager/ObjManager.h"
#include "../../HumimotoEngine/math/Matrix4x4.h"
#include "../../HumimotoEngine/VertexData.h"
#include "../../HumimotoEngine/Transform.h"
#include "../../HumimotoEngine/TransformationMatrix.h"
#include "../../HumimotoEngine/Material.h"
#include "../../HumimotoEngine/MaterialData.h"
#include "../../HumimotoEngine/Manager/TextureManager.h"
#include "../../HumimotoEngine/base/DirectXCommon.h"

class Block;

class Player {
public: // メンバ関数
	ModelData GetModelData() { return modelData_; }

	void Initialize(Vector3 position);	// 初期化
	void Update();	// 更新
	void Draw();	// 描画

	// つかみを無理やり解除
	void ReleaseGrab();

private: // メンバ定数
	// プレイヤーの移動速度
	const float kPlayerMovementSpeed = 0.2f;
	// プレイヤーの当たり判定
	float kPlayerCollisionSize_ = 1.0f;

public:
	// SRT
	Transform transform;
	// テクスチャナンバー
	int textureNum;

	// 現在の座標（ブロックつかみ中の徐々に移動する処理を実装するため）
	Vector3 currentPosition;

private:
	// トリガー用の変数
	bool preLT = false;
	bool preRT = false;
	// ブロックのポインタ
	Block* blockPtr_ = nullptr;

	// ブロックつかみ中
	bool isGrabbing = false;

private: // プライベートな変数と関数
	// Material
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
	// Vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	// カメラ
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;

	// モデルのポインタ
	ModelData modelData_;
	// ワールドトランスフォーム
	//WorldTransform worldTransform_;

	// モデル用の処理
	void BaseInitialize();	// 初期化
	void BaseDraw();	// 描画

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);
	void CreateVertexResource();
	void CreateVertexBufferView();
	void CreateMaterialResource();
	void CreateWvpResource();
};