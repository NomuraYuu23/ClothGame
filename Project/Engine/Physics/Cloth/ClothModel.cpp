#include "ClothModel.h"
#include "../../base/DxCommon/DirectXCommon.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../base/Texture/TextureManager.h"

//	平行光源
DirectionalLight* ClothModel::sDirectionalLight_ = nullptr;
// ポイントライトマネージャ
PointLightManager* ClothModel::sPointLightManager_ = nullptr;
//	スポットライトマネージャ
SpotLightManager* ClothModel::sSpotLightManager_ = nullptr;
// 霧マネージャー
FogManager* ClothModel::sFogManager_ = nullptr;

void ClothModel::StaticInitialize(
	DirectionalLight* sDirectionalLight, 
	PointLightManager* sPointLightManager, 
	SpotLightManager* sSpotLightManager, 
	FogManager* sFogManager)
{

	//	平行光源
	sDirectionalLight_ = sDirectionalLight;
	// ポイントライトマネージャ
	sPointLightManager_ = sPointLightManager;
	//	スポットライトマネージャ
	sSpotLightManager_ = sSpotLightManager;
	// 霧マネージャー
	sFogManager_ = sFogManager;

}

void ClothModel::Initialize(const Vector2& div)
{

	// 分割数
	div_ = div;

	// 頂点の数
	vertexNum_ = (static_cast<uint32_t>(div_.x) + 1) * (static_cast<uint32_t>(div_.y) + 1);

	// インデックスの数
	indexNum_ = static_cast<uint32_t>(div_.x) * static_cast<uint32_t>(div_.y) * 6;

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	// 頂点バッファ
	vertBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(ClothModel::VertexData) + 0xff) & ~0xff) * vertexNum_);
	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vbView_.SizeInBytes = UINT(sizeof(ClothModel::VertexData) * vertexNum_);
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(ClothModel::VertexData);
	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap_));
	// 一度0でマッピング
	VertexMapping();

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(uint32_t) + 0xff) & ~0xff) * indexNum_);
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * indexNum_;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap_));

	// インデックスマッピング
	IndexMapping();

	// テクスチャハンドル
	textureHandle_ = TextureManager::Load("Resources/default/clothDemo.png", DirectXCommon::GetInstance());

	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(HalfLambert);

	// トランスフォームバッファ
	wvpBuff_ = BufferResource::CreateBufferResource(
		device, ((sizeof(ClothModel::WVP) + 0xff) & ~0xff));
	// トランスフォーム行列マップ
	wvpBuff_->Map(0, nullptr, reinterpret_cast<void**>(&wvpMap_));

	// トランスフォームマップ
	wvpMap_->matrix_ = Matrix4x4::MakeIdentity4x4();

}

void ClothModel::Update(const std::vector<Vector3>& positions)
{

	std::vector<ClothModel::Surface> surfaces;
	// 面情報
	for (uint32_t y = 0; y < static_cast<uint32_t>(div_.y); ++y) {
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x); ++x) {

			Surface surface{};

			// 左上
			surface.indexes_[0] = y * (static_cast<uint32_t>(div_.x) + 1) + x;
			Vector3 leftTop = positions[surface.indexes_[0]];
			// 右上
			surface.indexes_[1] = y * (static_cast<uint32_t>(div_.x) + 1) + x + 1;
			Vector3 rightTop = positions[surface.indexes_[1]];
			// 左下
			surface.indexes_[2] = (y + 1) * (static_cast<uint32_t>(div_.x) + 1) + x;
			Vector3 leftBottom = positions[surface.indexes_[2]];
			// 右下
			surface.indexes_[3] = (y + 1) * (static_cast<uint32_t>(div_.x) + 1) + x + 1;
			Vector3 rightBottom = positions[surface.indexes_[3]];

			// 法線作成

			// 1つ目の三角形のクロス積を計算する
			Vector3	cross0 = Vector3::Cross(rightBottom - rightTop, leftBottom - rightTop);
			// 2つ目の三角形
			Vector3	cross1 = Vector3::Cross(leftBottom - rightTop, leftTop - rightTop);
			// 2つのクロス積の結果を合わせる
			Vector3	normal = cross0 + cross1;
			// 単位ベクトルへ正則化
			normal = Vector3::Normalize(normal);
			surface.normal_ = normal;

			surfaces.push_back(surface);

		}
	}

	for (uint32_t i = 0; i < vertexNum_; ++i) {
		// 頂点マッピング
		vertMap_[i].position_.x = positions[i].x;
		vertMap_[i].position_.y = positions[i].y;
		vertMap_[i].position_.z = positions[i].z;
		vertMap_[i].position_.w = 1.0f;
		// 法線マッピング
		std::array<Vector3, 4> normals{};
		uint32_t normalNum = 0;

		for (uint32_t j = 0; j < surfaces.size(); ++j) {
			for (uint32_t k = 0; k < 4; ++k) {
				if (surfaces[j].indexes_[k] == i) {
					normals[normalNum] = surfaces[j].normal_;
					normalNum++;
					break;
				}
			}

			if (normalNum >= 4) {
				break;
			}

		}

		Vector3 normal = {0.0f,0.0f,0.0f};
		for (uint32_t j = 0; j < normalNum; ++j) {
			normal = normals[j];
		}

		vertMap_[i].normal_ = Vector3::Normalize(normal);

	}

}

void ClothModel::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera)
{

	// nullptrチェック
	assert(commandList);
	assert(sDirectionalLight_);
	assert(sPointLightManager_);
	assert(sSpotLightManager_);
	assert(sFogManager_);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->IASetVertexBuffers(0, 1, &vbView_);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マップ
	wvpMap_->matrix_ = camera->GetViewProjectionMatrix();

	// パイプライン設定
	commandList->SetPipelineState(GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexCloth].Get());//PS0を設定
	commandList->SetGraphicsRootSignature(GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexCloth].Get());

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView_);
	//IBVを設定
	commandList->IASetIndexBuffer(&ibView_);

	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	// 平行光源
	sDirectionalLight_->Draw(commandList, 1);

	// カメラCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(2, camera->GetWorldPositionBuff()->GetGPUVirtualAddress());

	// ワールドトランスフォーム
	commandList->SetGraphicsRootConstantBufferView(3, wvpBuff_->GetGPUVirtualAddress());

	//テクスチャ 
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, 4, textureHandle_);

	// ポイントライト
	sPointLightManager_->Draw(commandList, 5);
	// スポットライト
	sSpotLightManager_->Draw(commandList, 6);

	// 霧
	commandList->SetGraphicsRootConstantBufferView(7, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	//描画
	commandList->DrawIndexedInstanced(indexNum_, 1, 0, 0, 0);

}

void ClothModel::VertexMapping()
{

	for (uint32_t y = 0; y < static_cast<uint32_t>(div_.y) + 1; ++y) {
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x) + 1; ++x) {
			uint32_t index = y * (static_cast<uint32_t>(div_.x) + 1) + x;
			vertMap_[index].position_ = { 0.0f,0.0f,0.0f,1.0f };
			vertMap_[index].texcoord_ = { static_cast<float>(x) / (div_.x), static_cast<float>(y) / (div_.y) };
			vertMap_[index].normal_ = { 0.0f, 0.0f, -1.0f };
		}
	}
}

void ClothModel::IndexMapping()
{

	uint32_t index = 0;

	for(uint32_t y = 0; y < static_cast<uint32_t>(div_.y); ++y){
		for (uint32_t x = 0; x < static_cast<uint32_t>(div_.x); ++x) {

			// 一つ目の三角形
			// 左上
			indexMap_[index] = y * (static_cast<uint32_t>(div_.x) + 1) + x;
			index++;
			// 右上
			indexMap_[index] = y * (static_cast<uint32_t>(div_.x) + 1) + x + 1;
			index++;
			// 左下
			indexMap_[index] = (y + 1) * (static_cast<uint32_t>(div_.x) + 1) + x;
			index++;

			// 二つ目の三角形
			// 右上
			indexMap_[index] = y * (static_cast<uint32_t>(div_.x) + 1) + x + 1;
			index++;
			// 左下
			indexMap_[index] = (y + 1) * (static_cast<uint32_t>(div_.x) + 1) + x;
			index++;
			// 右下
			indexMap_[index] = (y + 1) * (static_cast<uint32_t>(div_.x) + 1) + x + 1;
			index++;
		
		}
	}


}
