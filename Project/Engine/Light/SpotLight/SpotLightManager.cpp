#include "SpotLightManager.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../3D/Transform/TransformationMatrix.h"
#include "../../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* SpotLightManager::sDevice_ = nullptr;
// コマンドリスト
ID3D12GraphicsCommandList* SpotLightManager::sCommandList_ = nullptr;

void SpotLightManager::StaticInitialize(ID3D12Device* device)
{

	assert(device);

	sDevice_ = device;

}

SpotLightManager::~SpotLightManager()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(indexDescriptorHeap_);

}

void SpotLightManager::Initialize()
{

	//平行光源リソースを作る
	spotLightDataBuff_ = BufferResource::CreateBufferResource(sDevice_, ((sizeof(SpotLightData) + 0xff) & ~0xff) * kNumInstanceMax_);

	//書き込むためのアドレスを取得
	spotLightDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightDataMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		spotLightDataMap_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		spotLightDataMap_[i].position = { 0.0f, -1.0f, 0.0f };
		spotLightDataMap_[i].intencity = 1.0f;
		spotLightDataMap_[i].direction = { 0.0f, -1.0f, 0.0f }; // ライトの方向
		spotLightDataMap_[i].distance = 10.0f; // ライトの届く距離
		spotLightDataMap_[i].decay = 2.0f; // 減衰率
		spotLightDataMap_[i].cosAngle = 2.0f; // スポットライトの余弦
		spotLightDataMap_[i].cosFalloffStart = 1.0f; // フォールオフ開始位置
		spotLightDataMap_[i].used = false; // 使用している
	}

	SRVCreate();

}

void SpotLightManager::SRVCreate()
{

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(SpotLightData);
	instancingSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	instancingSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(spotLightDataBuff_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}

void SpotLightManager::Update(const std::array<SpotLightData, SpotLightManager::kNumInstanceMax_>& spotLightDatas)
{

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		spotLightDataMap_[i].color = spotLightDatas[i].color;
		spotLightDataMap_[i].position = spotLightDatas[i].position;
		spotLightDataMap_[i].intencity = spotLightDatas[i].intencity;
		spotLightDataMap_[i].direction = Vector3::Normalize(spotLightDatas[i].direction);
		spotLightDataMap_[i].distance = spotLightDatas[i].distance;
		spotLightDataMap_[i].decay = spotLightDatas[i].decay; 
		spotLightDataMap_[i].cosAngle = spotLightDatas[i].cosAngle; 
		spotLightDataMap_[i].cosFalloffStart = spotLightDatas[i].cosFalloffStart; 
		spotLightDataMap_[i].used = spotLightDatas[i].used;
	}

}

void SpotLightManager::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex)
{

	assert(sCommandList_ == nullptr);

	sCommandList_ = cmdList;

	sCommandList_->SetGraphicsRootDescriptorTable(rootParameterIndex, instancingSrvHandleGPU_);

	// コマンドリストを解除
	sCommandList_ = nullptr;

}
