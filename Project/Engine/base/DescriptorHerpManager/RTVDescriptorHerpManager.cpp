#include "RTVDescriptorHerpManager.h"

// ディスクリプタヒープ
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVDescriptorHerpManager::descriptorHeap_ = nullptr;
// デバイス
ID3D12Device* RTVDescriptorHerpManager::device_ = nullptr;
// 次のディスクリプタヒープの場所
uint32_t RTVDescriptorHerpManager::nextIndexDescriptorHeap_ = 0u;
// ディスクリプタヒープのどこが空いているか
std::array<bool, RTVDescriptorHerpManager::kNumDescriptors> RTVDescriptorHerpManager::isNullDescriptorHeaps_;

RTVDescriptorHerpManager* RTVDescriptorHerpManager::GetInstance()
{

	static RTVDescriptorHerpManager instance;
	return &instance;

}

void RTVDescriptorHerpManager::Initialize(ID3D12Device* device)
{

	HRESULT result = S_FALSE;

	device_ = device;

	// ディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descHeapDesc.NumDescriptors = kNumDescriptors;
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));

	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		isNullDescriptorHeaps_[i] = true;
	}

}

D3D12_CPU_DESCRIPTOR_HANDLE RTVDescriptorHerpManager::GetCPUDescriptorHandle()
{

	uint32_t descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * nextIndexDescriptorHeap_);
	return handleCPU;

}

void RTVDescriptorHerpManager::NextIndexDescriptorHeapChange()
{

	isNullDescriptorHeaps_[nextIndexDescriptorHeap_] = false;

	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		if (isNullDescriptorHeaps_[i]) {
			nextIndexDescriptorHeap_ = static_cast<uint32_t>(i);
			break;
		}
	}

}

void RTVDescriptorHerpManager::DescriptorHeapsMakeNull(uint32_t index)
{
	isNullDescriptorHeaps_[index] = true;
}
