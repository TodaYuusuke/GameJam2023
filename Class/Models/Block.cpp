#include "Block.h"

Microsoft::WRL::ComPtr<ID3D12Resource> Block::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
	HRESULT hr;
	// ���_���\�[�X�p�̃q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeap��g��
	// ���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// �o�b�t�@�\�[�X�B�e�N�X�`���̏ꍇ�͂܂��ʂ̐ݒ�����
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // ���\�[�X�̃T�C�Y�B�����Vector4��3���_��
	// �o�b�t�@�̏ꍇ�͂��ꂩ���1�ɂ��錈�܂�
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// �o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// ���ۂɒ��_���\�[�X����
	hr = device.Get()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;
}

void Block::CreateVertexResource() {
	vertexResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(VertexData) * modelData_.vertices.size()).Get();
}

void Block::CreateVertexBufferView() {
	// ���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	// �g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * UINT(modelData_.vertices.size());
	// 1���_������̃T�C�Y
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Block::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(Material)).Get();
	// �}�e���A���Ƀf�[�^���������
	materialData_ = nullptr;
	// �������ނ��߂̃A�h���X��擾
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Block::CreateWvpResource() {
	// 1���̃T�C�Y��p�ӂ���
	transformationMatrixResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix)).Get();
	// �������ނ��߂̃A�h���X��擾
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// �P�ʍs����������ł���
	transformationMatrixData_->WVP = MakeIdentity4x4();
}

void Block::Initialize() {
	// ���f����ǂݍ���
	modelData_ = ObjManager::GetInstance()->GetObjModelData()[block];

	CreateVertexResource();

	CreateMaterialResource();

	CreateWvpResource();

	CreateVertexBufferView();

	// �������ނ��߂̃A�h���X��擾
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	uvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	// Lighting���邩
	materialData_->enableLighting = false;

	// uvTransform�s��̏�����
	materialData_->uvTransform = MakeIdentity4x4();
}

void Block::Draw(Vector3 translate, Vector3 scele, Vector3 rotate, int textureNum) {
	transform_.translate = translate;
	transform_.scale = scele;
	transform_.rotate = rotate;

	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	// �J����
	//transform_.rotate.y += 0.006f;
	transformationMatrixData_->World = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	transformationMatrixData_->WVP = Multiply(transformationMatrixData_->World, *Camera::GetInstance()->GetTransformationMatrixData());
	transformationMatrixData_->World = MakeIdentity4x4();

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };


	// �R�}���h��ς�
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBV��ݒ�
	// �`���ݒ�BPSO�ɐݒ肵�Ă����̂Ƃ͂܂��ʁB������̂�ݒ肷��ƍl���Ă����Ηǂ�
	DirectXCommon::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// DescriptorTable�̐ݒ�
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureSrvHandleGPU()[textureNum]);

	// wvp��CBuffer�̏ꏊ��ݒ�
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_.Get()->GetGPUVirtualAddress());

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());

	// �}�e���A��CBuffer�̏ꏊ��ݒ�
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());
	DirectXCommon::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}
