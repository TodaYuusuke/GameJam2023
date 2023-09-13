#include "Player.h"
#include "../Map/MapManager.h"
#include "../../HumimotoEngine/components/Input.h"
#include "../../HumimotoEngine/components/camera/DebugCamera.h"
#include "../Utility/MyUtility.h"

// トリガーボタン (LT および RT)
#define XINPUT_GAMEPAD_LEFT_TRIGGER    0x00FF
#define XINPUT_GAMEPAD_RIGHT_TRIGGER   0xFF00

void Player::Initialize(Vector3 position) {
	transform.translate = position;
	transform.rotate = { 0.0f,0.0f,0.0f };
	float scale = MapManager::GetInstance()->GetCurrentMap()->GetBlockScale();
	transform.scale = { scale,scale,scale };

	textureNum = PLAYER;

	BaseInitialize();
}
void Player::Update() {
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// ブロックをつかんでいない間の移動処理
		if (!isGrabbing) {
			// デッドゾーンの設定
			SHORT leftThumbX = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLX);
			SHORT leftThumbY = Input::GetInstance()->ApplyDeadzone(joyState.Gamepad.sThumbLY);

			// プレイヤー移動
			Vector3 move = {
				(float)leftThumbX / SHRT_MAX,
				0.0f,
				(float)leftThumbY / SHRT_MAX
			};

			Vector3 rotation = DebugCamera::GetInstance()->GetCameraRotation();
			rotation.x = 0.0f;	// X軸は捨てる
			move = move * MakeRotateMatrix(rotation);
			move.y = 0.0f;	// Y軸は捨てる
			Vector3 nextPos = transform.translate + (move * kPlayerMovementSpeed * transform.scale.x);
			Vector3 resultPos = nextPos;

			// 移動先が落下しないかを確認
			if (!MapManager::GetInstance()->GetCurrentMap()->IsCollisionBool({ nextPos.x,nextPos.y - 2.0f * transform.scale.x,nextPos.z })) {
				if (MapManager::GetInstance()->GetCurrentMap()->IsCollisionBool({ nextPos.x,nextPos.y - 4.0f * transform.scale.x,nextPos.z })) {
					nextPos.y -= 2.0f * transform.scale.x;
					resultPos.y -= 2.0f * transform.scale.x;
				}
				else {
					if (MapManager::GetInstance()->GetCurrentMap()->IsCollisionBool({ nextPos.x ,nextPos.y - 2.0f * transform.scale.x, transform.translate.z })) {
						resultPos.z = transform.translate.z;
					}
					else {
						resultPos.x = transform.translate.x;
					}
					if (MapManager::GetInstance()->GetCurrentMap()->IsCollisionBool({ transform.translate.x,nextPos.y - 2.0f * transform.scale.x, nextPos.z })) {
						resultPos.x = transform.translate.x;
					}
					else {
						resultPos.z = transform.translate.z;
					}
				}
			}
			transform.translate = resultPos;

			// キャラクターの向きを決定する
			if (Length(move) > 0.0f) {
				// 目的の角度
				Vector3 goalRotation = { 0.0f, 0.0f, 0.0f };
				// Y軸周りの角度
				goalRotation.y = std::atan2f(move.x, move.z);
				// X軸周りの角度
				goalRotation.x = std::atan2f(-move.y, Length({ move.x, 0, move.z }));

				// 現在の角度と目標の角度を比較し、逆回転の場合に調整
				if (std::abs(transform.rotate.y - goalRotation.y) > M_PI) {
					if (transform.rotate.y > goalRotation.y) {
						transform.rotate.y -= static_cast<float>(2.0f * M_PI);
					}
					else {
						transform.rotate.y += static_cast<float>(2.0f * M_PI);
					}
				}

				// 角度を適応
				transform.rotate = Utility::Slerp(transform.rotate, goalRotation, 0.2f);
			}
		}

		// ブロックをつかめるかチェック
		// LT or RTトリガーを押していたら >>> ブロックをつかむ
		if ((!preLT && joyState.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_TRIGGER) || (!preRT && joyState.Gamepad.bRightTrigger > 0)) {
			if (!isGrabbing && MapManager::GetInstance()->GetCurrentMap()->GrabBlock(this)) {
				isGrabbing = true;
			}
			else if (isGrabbing){
				ReleaseGrab();
			}
		}

		// トリガー用のbool
		preLT = joyState.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_TRIGGER;
		preRT = joyState.Gamepad.bRightTrigger > 0;
	}

	// 当たり判定をチェック
	AABB collision;
	Vector3 size = { kPlayerCollisionSize_ * transform.scale.x, kPlayerCollisionSize_ * transform.scale.y, kPlayerCollisionSize_ * transform.scale.z };
	collision.min = transform.translate - size;
	collision.max = transform.translate + size;
	collision.min.y += 0.010f;
	collision.max.y -= 0.100f;
	transform.translate += MapManager::GetInstance()->GetCurrentMap()->IsCollisionVector3(collision);
}
void Player::Draw() {
	BaseDraw();
}

void Player::ReleaseGrab(){
	MapManager::GetInstance()->GetCurrentMap()->ReleaseBlock();
	isGrabbing = false;
}


void Player::BaseInitialize() {
	// ���f����ǂݍ���
	modelData_ = ObjManager::GetInstance()->GetObjModelData()[player];

	CreateVertexResource();

	CreateMaterialResource();

	CreateWvpResource();

	CreateVertexBufferView();

	// �������ނ��߂̃A�h���X��擾
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

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

void Player::BaseDraw() {
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	// �J����
	//transform_.rotate.y += 0.006f;
	transformationMatrixData_->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

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

Microsoft::WRL::ComPtr<ID3D12Resource> Player::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
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

void Player::CreateVertexResource() {
	vertexResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(VertexData) * modelData_.vertices.size()).Get();
}

void Player::CreateVertexBufferView() {
	// ���\�[�X�̐擪�̃A�h���X����g��
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	// �g�p���郊�\�[�X�̃T�C�Y�͒��_3���̃T�C�Y
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * UINT(modelData_.vertices.size());
	// 1���_������̃T�C�Y
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Player::CreateMaterialResource() {
	materialResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(Material)).Get();
	// �}�e���A���Ƀf�[�^���������
	materialData_ = nullptr;
	// �������ނ��߂̃A�h���X��擾
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Player::CreateWvpResource() {
	// 1���̃T�C�Y��p�ӂ���
	transformationMatrixResource_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix)).Get();
	// �������ނ��߂̃A�h���X��擾
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	// �P�ʍs����������ł���
	transformationMatrixData_->WVP = MakeIdentity4x4();
}