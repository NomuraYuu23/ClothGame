#include "TextureUAV.h"
#include <cassert>

TextureUAV::~TextureUAV()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(uavIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(srvIndexDescriptorHeap_);

}

void TextureUAV::Initialize(
	ID3D12Device* device,
	int32_t width,
	int32_t height)
{

	D3D12_HEAP_PROPERTIES prop{};

	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Alignment = 0;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resourceDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	resourceDesc.Height = height;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc = { 1,0 };
	resourceDesc.Width = width;

	HRESULT hr = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&resource_));

	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.Texture2D.MipSlice = 0;
	desc.Texture2D.PlaneSlice = 0;

	uavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	uavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	uavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(
		resource_.Get(),
		nullptr,
		&desc,
		uavHandleCPU_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	srvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(resource_.Get(), &srvDesc, srvHandleCPU_);

}

void TextureUAV::SetRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex)
{

	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//シェーダーリソースビューをセット
	commandList->SetComputeRootDescriptorTable(rootParamIndex, uavHandleGPU_);

}

void TextureUAV::UavBarrier(ID3D12GraphicsCommandList* commandList)
{

	assert(commandList);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。
	barrier.UAV.pResource = resource_.Get();
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

}

void TextureUAV::ChangePixelShaderResource(ID3D12GraphicsCommandList* commandList)
{

	assert(commandList);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。
	barrier.Transition.pResource = resource_.Get();
	//遷移前（現在）のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	//遷移後のResoureState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

}

void TextureUAV::ChangeUnorderedAccessResource(ID3D12GraphicsCommandList* commandList)
{

	assert(commandList);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。
	barrier.Transition.pResource = resource_.Get();
	//遷移前（現在）のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//遷移後のResoureState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

}
