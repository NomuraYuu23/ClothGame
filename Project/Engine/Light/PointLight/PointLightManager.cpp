#include "PointLightManager.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../3D/Transform/TransformationMatrix.h"
#include "../../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* PointLightManager::sDevice = nullptr;
// コマンドリスト
ID3D12GraphicsCommandList* PointLightManager::sCommandList = nullptr;

void PointLightManager::StaticInitialize(ID3D12Device* device)
{

	assert(device);

	sDevice = device;

}

PointLightManager::~PointLightManager()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(indexDescriptorHeap_);

}

void PointLightManager::Initialize()
{

	//平行光源リソースを作る
	pointLightDataBuff_ = BufferResource::CreateBufferResource(sDevice, ((sizeof(PointLightData) + 0xff) & ~0xff) * kNumInstanceMax_);

	//書き込むためのアドレスを取得
	pointLightDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightDataMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		pointLightDataMap_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDataMap_[i].position = { 0.0f, -1.0f, 0.0f };
		pointLightDataMap_[i].intencity = 1.0f;
		pointLightDataMap_[i].radius = 10.0f;
		pointLightDataMap_[i].decay = 10.0f;
		pointLightDataMap_[i].used = false;
	}

	SRVCreate();

}

void PointLightManager::SRVCreate()
{

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(PointLightData);
	instancingSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	instancingSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(pointLightDataBuff_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}

void PointLightManager::Update(const std::array<PointLightData, PointLightManager::kNumInstanceMax_>& pointLightDatas)
{

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		pointLightDataMap_[i].color = pointLightDatas[i].color;
		pointLightDataMap_[i].position = pointLightDatas[i].position;
		pointLightDataMap_[i].intencity = pointLightDatas[i].intencity;
		pointLightDataMap_[i].radius = pointLightDatas[i].radius;
		pointLightDataMap_[i].decay = pointLightDatas[i].decay;
		pointLightDataMap_[i].used = pointLightDatas[i].used;
	}

}

void PointLightManager::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex)
{

	assert(sCommandList == nullptr);

	sCommandList = cmdList;

	sCommandList->SetGraphicsRootDescriptorTable(rootParameterIndex, instancingSrvHandleGPU_);

	// コマンドリストを解除
	sCommandList = nullptr;

}
