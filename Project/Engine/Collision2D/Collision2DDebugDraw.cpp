#include "Collision2DDebugDraw.h"
#include "../base/BufferResource.h"
#include "../base/Texture/TextureManager.h"
#include "../2D/ImguiManager.h"
#include <numbers>

Collision2DDebugDraw::~Collision2DDebugDraw()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(indexDescriptorHeap_);

}

void Collision2DDebugDraw::Initialize(ID3D12Device* device,
	const std::array<UINT, kTexutureNameOfCount>& textureHandles,
	ID3D12RootSignature* rootSignature,
	ID3D12PipelineState* pipelineState)
{

	device_ = device;
	rootSignature_ = rootSignature;
	pipelineState_ = pipelineState;

	isInvisible_ = true;

	viewMatrix_ = Matrix4x4::MakeIdentity4x4();
	projectionMatrix_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth_), float(WinApp::kWindowHeight_), 0.0f, 100.0f);

	// 頂点
	//Sprite用の頂点リソースを作る
	vertBuff_ = BufferResource::CreateBufferResource(device_, sizeof(VertexData) * 6);

	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vbView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap));

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(device_, sizeof(uint32_t) * 6);

	//インデックスバッファビュー
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));

	float left = -0.5f * textureScale.x;
	float right = 0.5f * textureScale.x;
	float top = -0.5f * textureScale.y;
	float bottom = 0.5f * textureScale.y;

	//一枚目の三角形
	vertMap[0].position = { left, bottom, 0.0f, 1.0f };//左下
	vertMap[0].texcoord = { 0.0f, 1.0f };
	vertMap[0].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[1].position = { left, top, 0.0f, 1.0f };//左上
	vertMap[1].texcoord = { 0.0f, 0.0f };
	vertMap[1].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[2].position = { right, bottom, 0.0f, 1.0f };//右下
	vertMap[2].texcoord = { 1.0f, 1.0f };
	vertMap[2].normal = { 0.0f, 0.0f, -1.0f };
	//ニ枚目の三角形
	vertMap[3].position = { right, top, 0.0f, 1.0f };//右上
	vertMap[3].texcoord = { 1.0f, 0.0f };
	vertMap[3].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[4].position = { left, top, 0.0f, 1.0f };//左上
	vertMap[4].texcoord = { 0.0f, 0.0f };
	vertMap[4].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[5].position = { right, bottom, 0.0f, 1.0f };//右下
	vertMap[5].texcoord = { 1.0f, 1.0f };
	vertMap[5].normal = { 0.0f, 0.0f, -1.0f };

	//インデックスリソースにデータを書き込む
	indexMap[0] = 0;
	indexMap[1] = 1;
	indexMap[2] = 2;
	indexMap[3] = 1;
	indexMap[4] = 3;
	indexMap[5] = 2;

	// テクスチャ
	textureHandles_ = textureHandles;

	for (size_t i = 0; i < textureHandles_.size(); ++i) {
		resourceDesc_[i] = TextureManager::GetInstance()->GetResourceDesc(textureHandles_[i]);
	}

	// トランスフォーム
	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	collider2DDebugDrawForGPUBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(Collider2DDebugDrawForGPU) + 0xff) & ~0xff) * kCollider2DDebugDrawForGPUNumMax_);
	//書き込むためのアドレスを取得
	collider2DDebugDrawForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&collider2DDebugDrawForGPUMap_));

	for (size_t i = 0; i < kCollider2DDebugDrawForGPUNumMax_; ++i) {
		collider2DDebugDrawForGPUMap_[i].World = Matrix4x4::MakeIdentity4x4();
		collider2DDebugDrawForGPUMap_[i].WVP = Matrix4x4::MakeIdentity4x4();
		collider2DDebugDrawForGPUMap_[i].textureNum = 0;
	}

	SRVCreate();

	// マテリアル
	material_.reset(Material::Create());

}

void Collision2DDebugDraw::Clear()
{

	collider2DDebugDrawForGPUNumCount_ = 0;

}

void Collision2DDebugDraw::Register(ColliderShape2D collider)
{

	Vector3 traslate = {};
	Vector3 roate = {};
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	if (std::holds_alternative<Box*>(collider)) {
		
		Box* obj = std::get<Box*>(collider);

		// 位置
		traslate.x = obj->position_.x;
		traslate.y = obj->position_.y;
		// 回転
		roate.z = obj->rotation_ * static_cast<float>(std::numbers::pi) / 180.0f;
		// スケール
		scale.x = obj->scale_.x / textureScale.x;
		scale.y = obj->scale_.y / textureScale.y;

		collider2DDebugDrawForGPUMap_[collider2DDebugDrawForGPUNumCount_].textureNum = 0;

	}
	else if(std::holds_alternative<Circle*>(collider)){
			
		Circle* obj = std::get<Circle*>(collider);

		// 位置
		traslate.x = obj->position_.x;
		traslate.y = obj->position_.y;
		// スケール
		scale.x = obj->scale_.x / textureScale.x;
		scale.y = obj->scale_.y / textureScale.y;

		collider2DDebugDrawForGPUMap_[collider2DDebugDrawForGPUNumCount_].textureNum = 1;
		
	}
	else {
		return;
	}

	Matrix4x4 transformMatrix = Matrix4x4::MakeAffineMatrix(scale, roate, traslate);

	Matrix4x4 worldViewProjectionMatrixSprite = Matrix4x4::Multiply(transformMatrix, Matrix4x4::Multiply(viewMatrix_, projectionMatrix_));

	collider2DDebugDrawForGPUMap_[collider2DDebugDrawForGPUNumCount_].World = transformMatrix;
	collider2DDebugDrawForGPUMap_[collider2DDebugDrawForGPUNumCount_].WVP = worldViewProjectionMatrixSprite;

	collider2DDebugDrawForGPUNumCount_++;

}

void Collision2DDebugDraw::Draw(ID3D12GraphicsCommandList* cmdList)
{

	// 非表示
	if (isInvisible_ || collider2DDebugDrawForGPUNumCount_ == 0) {
		return;
	}

	assert(commandList_ == nullptr);

	commandList_ = cmdList;

	//RootSignatureを設定。
	commandList_->SetPipelineState(pipelineState_);//PS0を設定
	commandList_->SetGraphicsRootSignature(rootSignature_);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vbView_);
	//IBVを設定
	commandList_->IASetIndexBuffer(&ibView_);

	//マテリアルCBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	//collider2DDebugDrawForGPUBuffの場所を設定
	commandList_->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	// シェーダーリソースビューをセット
	for (size_t i = 0; i < kTexutureNameOfCount; ++i) {
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, 2 + static_cast<UINT>(i), textureHandles_[i]);
	}
	//描画
	commandList_->DrawIndexedInstanced(6, collider2DDebugDrawForGPUNumCount_, 0, 0, 0);

	//コマンドリストを解除
	commandList_ = nullptr;

}

void Collision2DDebugDraw::ImGuiDraw()
{

	ImGui::Begin("Collision2DDebug");
	ImGui::Checkbox("表示しない", &isInvisible_);
	ImGui::End();

}

void Collision2DDebugDraw::SRVCreate()
{

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = static_cast<UINT>(kCollider2DDebugDrawForGPUNumMax_);
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(Collider2DDebugDrawForGPU);
	instancingSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	instancingSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(collider2DDebugDrawForGPUBuff_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}
