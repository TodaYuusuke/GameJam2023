#include "Block.h"

Microsoft::WRL::ComPtr<ID3D12Resource> Block::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
	HRESULT hr;
	//    _   \ [ X p ̃q [ v ̐ݒ 
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeap  g  
	//    _   \ [ X ̐ݒ 
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//  o b t @ \ [ X B e N X `   ̏ꍇ ͂܂  ʂ̐ݒ     
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; //    \ [ X ̃T C Y B     Vector4  3   _  
	//  o b t @ ̏ꍇ ͂  ꂩ   1 ɂ  錈 ܂ 
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//  o b t @ ̏ꍇ ͂   ɂ  錈 ܂ 
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	//    ۂɒ  _   \ [ X    
	hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void Block::CreateVertexResource() {
	vertexResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(VertexData) * 36));
}

void Block::CreateVertexBufferView() {
	//    \ [ X ̐擪 ̃A h   X    g  
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//  g p   郊 \ [ X ̃T C Y ͒  _3   ̃T C Y
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 36;
	// 1   _      ̃T C Y
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Block::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(Material)).Get();
	//  } e   A   Ƀf [ ^         
	materialData_ = nullptr;
	//        ނ  ߂̃A h   X  擾
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Block::CreateWvpResource() {
	// 1   ̃T C Y  p ӂ   
	transformationMatrixResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix)).Get();
	//        ނ  ߂̃A h   X  擾
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//  P ʍs          ł   
	transformationMatrixData_->WVP = MakeIdentity4x4();
}

void Block::Initialize() {
	CreateVertexResource();

	CreateMaterialResource();

	CreateWvpResource();

	CreateVertexBufferView();

	//        ނ  ߂̃A h   X  擾
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

#pragma region 正面

	// 左下
	vertexData_[0].position = { -0.5f, -0.5f, -0.5f,1.0f };
	vertexData_[0].texcoord = { 0.3333f,1.0f };
	// 左上:
	vertexData_[1].position = { -0.5f, 0.5f, -0.5f ,1.0f };
	vertexData_[1].texcoord = { 0.3333f,0.5f };
	// 右下
	vertexData_[2].position = { 0.5f, -0.5f, -0.5f,1.0f };
	vertexData_[2].texcoord = { 0.6666f,1.0f };

	// 左上
	vertexData_[3].position = { -0.5f, 0.5f, -0.5f ,1.0f };
	vertexData_[3].texcoord = { 0.3333f,0.5f };
	// 右上:
	vertexData_[4].position = { 0.5f, 0.5f, -0.5f ,1.0f };
	vertexData_[4].texcoord = { 0.6666f,0.5f };
	// 右下
	vertexData_[5].position = { 0.5f, -0.5f, -0.5f ,1.0f };
	vertexData_[5].texcoord = { 0.6666f,1.0f };

#pragma endregion

#pragma region 左面

	// 左下
	vertexData_[6].position = { -0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[6].texcoord = { 0.0f,0.5f };
	// 左上:
	vertexData_[7].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[7].texcoord = { 0.0f,0.0f };
	// 右下
	vertexData_[8].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[8].texcoord = { 0.3333f,0.5f };

	// 左上
	vertexData_[9].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[9].texcoord = { 0.0f,0.0f };
	// 右上:
	vertexData_[10].position = { -0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[10].texcoord = { 0.3333f,0.0f };
	// 右下
	vertexData_[11].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[11].texcoord = { 0.3333f,0.5f };

#pragma endregion

#pragma region 背面

	// 左下
	vertexData_[12].position = { 0.5f, -0.5f, 0.5f ,1.0f };
	vertexData_[12].texcoord = { 0.3333f,0.5f };
	// 左上:
	vertexData_[13].position = { 0.5f, 0.5f, 0.5f ,1.0f };
	vertexData_[13].texcoord = { 0.3333f,0.0f };
	// 右下
	vertexData_[14].position = { -0.5f, -0.5f, 0.5f ,1.0f };
	vertexData_[14].texcoord = { 0.6666f,0.5f };

	// 左上
	vertexData_[15].position = { 0.5f, 0.5f, 0.5f ,1.0f };
	vertexData_[15].texcoord = { 0.3333f,0.0f };
	// 右上:
	vertexData_[16].position = { -0.5f, 0.5f, 0.5f ,1.0f };
	vertexData_[16].texcoord = { 0.6666f,0.0f };
	// 右下
	vertexData_[17].position = { -0.5f, -0.5f, 0.5f ,1.0f };
	vertexData_[17].texcoord = { 0.6666f,0.5f };

#pragma endregion

#pragma region 上面

	// 左下
	vertexData_[18].position = { 0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[18].texcoord = { 0.6666f,1.0f };
	// 左上:
	vertexData_[19].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[19].texcoord = { 0.6666f,0.5f };
	// 右下
	vertexData_[20].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[20].texcoord = { 1.0f,1.0f };

	// 左上
	vertexData_[21].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[21].texcoord = { 0.6666f,0.5f };
	// 右上:
	vertexData_[22].position = { -0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[22].texcoord = { 1.0f,0.5f };
	// 右下
	vertexData_[23].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[23].texcoord = { 1.0f,1.0f };

#pragma endregion

#pragma region 右面

	// 左下
	vertexData_[24].position = { 0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[24].texcoord = { 0.0f,1.0f };
	// 左上:
	vertexData_[25].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[25].texcoord = { 0.0f,0.5f };
	// 右下
	vertexData_[26].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[26].texcoord = { 0.3333f,1.0f };

	// 左上
	vertexData_[27].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertexData_[27].texcoord = { 0.0f,0.5f };
	// 右上:
	vertexData_[28].position = { 0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[28].texcoord = { 0.3333f,0.5f };
	// 右下
	vertexData_[29].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[29].texcoord = { 0.3333f,1.0f };

#pragma endregion

#pragma region 底面

	// 左下
	vertexData_[30].position = { 0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[30].texcoord = { 0.6666f,0.5f };
	// 左上:
	vertexData_[31].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[31].texcoord = { 0.6666f,0.0f };
	// 右下
	vertexData_[32].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[32].texcoord = { 1.0f,0.5f };

	// 左上
	vertexData_[33].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[33].texcoord = { 0.6666f,0.0f };
	// 右上:
	vertexData_[34].position = { -0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[34].texcoord = { 1.0f,0.0f };
	// 右下
	vertexData_[35].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertexData_[35].texcoord = { 1.0f,0.5f };

#pragma endregion

	
	for (int i = 0; i < 36; i++) {
		// 法線の向き
		//vertexData_[i].normal = { vertexData_[i].position.x,vertexData_[i].position.y,vertexData_[i].position.z };
		// 頂点の位置が間違っていたのでx2した
		vertexData_[i].position.x *= 2;
		vertexData_[i].position.y *= 2;
		vertexData_[i].position.z *= 2;
	}

	// Lighting   邩
	materialData_->enableLighting = false;

	// uvTransform s  ̏     
	materialData_->uvTransform = MakeIdentity4x4();
}

void Block::Draw() {
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	//  J    
	//transform_.rotate.y += 0.006f;
	transformationMatrixData_->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	transformationMatrixData_->WVP = Multiply(transformationMatrixData_->World, *Camera::GetInstance()->GetTransformationMatrixData());
	transformationMatrixData_->World = MakeIdentity4x4();

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

	//  R }   h  ς 
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBV  ݒ 
	//  `   ݒ BPSO ɐݒ肵 Ă    ̂Ƃ͂܂  ʁB      ̂ ݒ肷  ƍl   Ă    Ηǂ 
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// DescriptorTable ̐ݒ 
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureSrvHandleGPU()[textureNum]);

	// wvp  CBuffer ̏ꏊ  ݒ 
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());

	//  } e   A  CBuffer ̏ꏊ  ݒ 
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced(36, 1, 0, 0);
}
