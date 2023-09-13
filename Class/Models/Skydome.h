#pragma once
#include "../../HumimotoEngine/math/Matrix4x4.h"
#include "../../HumimotoEngine/VertexData.h"
#include "../../HumimotoEngine/Transform.h"
#include "../../HumimotoEngine/TransformationMatrix.h"
#include "../../HumimotoEngine/Material.h"
#include "../../HumimotoEngine/MaterialData.h"
#include <d3d12.h>
#include "../../HumimotoEngine/Manager/TextureManager.h"
#include "../../HumimotoEngine/base/DirectXCommon.h"

class Skydome
{
public:
	ModelData GetModelData() { return modelData_; }

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void CreateMaterialResource();

	void CreateWvpResource();

	void Initialize();

	void Draw();

	void Release();

	void ApplyGlobalVariables();

	void ImGuiAdjustParameter();

public:
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
	Transform transform_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;

	ModelData modelData_;

	bool isAlive_ = true;
};

