#include "DSVDescriptorHerpManager.h"

// ディスクリプタヒープ
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVDescriptorHerpManager::descriptorHeap_ = nullptr;
// デバイス
ID3D12Device* DSVDescriptorHerpManager::device_ = nullptr;
// 次のディスクリプタヒープの場所
uint32_t DSVDescriptorHerpManager::nextIndexDescriptorHeap_ = 0u;
// ディスクリプタヒープのどこが空いているか
std::array<bool, DSVDescriptorHerpManager::kNumDescriptors_> DSVDescriptorHerpManager::isNullDescriptorHeaps_;

DSVDescriptorHerpManager* DSVDescriptorHerpManager::GetInstance()
{

	static DSVDescriptorHerpManager instance;
	return &instance;

}

void DSVDescriptorHerpManager::Initialize(ID3D12Device* device)
{

	HRESULT result = S_FALSE;

	device_ = device;

	// ディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descHeapDesc.NumDescriptors = kNumDescriptors_;
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));

	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		isNullDescriptorHeaps_[i] = true;
	}

}

D3D12_CPU_DESCRIPTOR_HANDLE DSVDescriptorHerpManager::GetCPUDescriptorHandle()
{

	uint32_t descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * nextIndexDescriptorHeap_);
	return handleCPU;

}

void DSVDescriptorHerpManager::NextIndexDescriptorHeapChange()
{

	isNullDescriptorHeaps_[nextIndexDescriptorHeap_] = false;

	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		if (isNullDescriptorHeaps_[i]) {
			nextIndexDescriptorHeap_ = static_cast<uint32_t>(i);
			break;
		}
	}

}

void DSVDescriptorHerpManager::DescriptorHeapsMakeNull(uint32_t index)
{
	isNullDescriptorHeaps_[index] = true;
}
