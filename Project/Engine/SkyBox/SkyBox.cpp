#include "Skybox.h"
#include "../base/DxCommon/DirectXCommon.h"
#include "../base/BufferResource.h"
#include "../base/Texture/TextureManager.h"

const size_t Skybox::kVertNum = 8;
const size_t Skybox::kIndexNum = 36;

void Skybox::Initialize(uint32_t textureHandle, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState)
{

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点バッファ
	vertBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(Skybox::VertexData) + 0xff) & ~0xff) * kVertNum);
	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vbView_.SizeInBytes = UINT(sizeof(Skybox::VertexData) * kVertNum);
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(Skybox::VertexData);
	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap_));

	VertMapping();

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(uint32_t) + 0xff) & ~0xff) * kIndexNum);
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * kIndexNum;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap_));

	IndexMapping();

	// トランスフォーム
	transform_.scale = { 100.0f,100.0f,100.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// トランスフォームバッファ
	skyboxForGPUBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(skyboxForGPUMap_) + 0xff) & ~0xff));
	// トランスフォーム行列マップ
	skyboxForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&skyboxForGPUMap_));

	// トランスフォームマップ
	transformMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	skyboxForGPUMap_->WVP = transformMatrix_;

	// マテリアル
	material_.reset(Material::Create());

	// テクスチャハンドル
	textureHandle_ = textureHandle;

	// ルートシグネチャ
	rootSignature_ = rootSignature;

	// パイプライン
	pipelineState_ = pipelineState;

}

void Skybox::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera)
{

	// nullptrチェック
	assert(commandList);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マップ
	transformMatrix_ = Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	skyboxForGPUMap_->WVP = transformMatrix_ * camera->GetViewProjectionMatrix();

	// パイプライン設定
	commandList->SetPipelineState(pipelineState_);//PS0を設定
	commandList->SetGraphicsRootSignature(rootSignature_);

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView_);
	//IBVを設定
	commandList->IASetIndexBuffer(&ibView_);

	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	//テクスチャ 
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, 1, textureHandle_);

	// ワールドトランスフォーム
	commandList->SetGraphicsRootConstantBufferView(2, skyboxForGPUBuff_->GetGPUVirtualAddress());

	//描画
	commandList->DrawIndexedInstanced(kIndexNum, 1, 0, 0, 0);

}

void Skybox::VertMapping()
{

#pragma region 右
	vertMap_[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertMap_[1].position = { 1.0f, 1.0f,-1.0f, 1.0f };
	vertMap_[2].position = { 1.0f,-1.0f, 1.0f, 1.0f };
	vertMap_[3].position = { 1.0f,-1.0f,-1.0f, 1.0f };
#pragma endregion

#pragma region 左
	vertMap_[4].position = { -1.0f, 1.0f,-1.0f, 1.0f };
	vertMap_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertMap_[6].position = { -1.0f,-1.0f,-1.0f, 1.0f };
	vertMap_[7].position = { -1.0f,-1.0f, 1.0f, 1.0f };
#pragma endregion

}

void Skybox::IndexMapping()
{

#pragma region 右
	indexMap_[0] = 0;
	indexMap_[1] = 1;
	indexMap_[2] = 2;

	indexMap_[3] = 2;
	indexMap_[4] = 1;
	indexMap_[5] = 3;
#pragma endregion

#pragma region 左
	indexMap_[6] = 4;
	indexMap_[7] = 5;
	indexMap_[8] = 6;

	indexMap_[9] = 6;
	indexMap_[10] = 5;
	indexMap_[11] = 7;
#pragma endregion

#pragma region 前
	indexMap_[12] = 1;
	indexMap_[13] = 4;
	indexMap_[14] = 3;

	indexMap_[15] = 3;
	indexMap_[16] = 4;
	indexMap_[17] = 6;
#pragma endregion

#pragma region 後
	indexMap_[18] = 2;
	indexMap_[19] = 7;
	indexMap_[20] = 0;

	indexMap_[21] = 0;
	indexMap_[22] = 7;
	indexMap_[23] = 5;
#pragma endregion

#pragma region 上
	indexMap_[24] = 0;
	indexMap_[25] = 5;
	indexMap_[26] = 1;

	indexMap_[27] = 1;
	indexMap_[28] = 5;
	indexMap_[29] = 4;
#pragma endregion

#pragma region 下
	indexMap_[30] = 3;
	indexMap_[31] = 6;
	indexMap_[32] = 2;

	indexMap_[33] = 2;
	indexMap_[34] = 6;
	indexMap_[35] = 7;
#pragma endregion

}
