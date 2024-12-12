#include "SRVDescriptorHerpManager.h"

// ディスクリプタヒープ
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVDescriptorHerpManager::descriptorHeap_ = nullptr;
// デバイス
ID3D12Device* SRVDescriptorHerpManager::device_ = nullptr;
// 次のディスクリプタヒープの場所
uint32_t SRVDescriptorHerpManager::nextIndexDescriptorHeap_ = 0u;
// ディスクリプタヒープのどこが空いているか
std::array<bool, SRVDescriptorHerpManager::kNumDescriptors_> SRVDescriptorHerpManager::isNullDescriptorHeaps_;

SRVDescriptorHerpManager* SRVDescriptorHerpManager::GetInstance()
{
	static SRVDescriptorHerpManager instance;
	return &instance;
}

void SRVDescriptorHerpManager::Initialize(ID3D12Device* device)
{

	HRESULT result = S_FALSE;

	device_ = device;

	// ディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = kNumDescriptors_;
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(result));

	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		isNullDescriptorHeaps_[i] = true;
	}

}

D3D12_CPU_DESCRIPTOR_HANDLE SRVDescriptorHerpManager::GetCPUDescriptorHandle()
{

	uint32_t descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * nextIndexDescriptorHeap_);
	return handleCPU;

}

D3D12_GPU_DESCRIPTOR_HANDLE SRVDescriptorHerpManager::GetGPUDescriptorHandle()
{

	uint32_t descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * nextIndexDescriptorHeap_);
	return handleGPU;

}

void SRVDescriptorHerpManager::NextIndexDescriptorHeapChange()
{

	isNullDescriptorHeaps_[nextIndexDescriptorHeap_] = false;
	
	for (size_t i = 0; i < isNullDescriptorHeaps_.size(); ++i) {
		if (isNullDescriptorHeaps_[i]) {
			nextIndexDescriptorHeap_ = static_cast<uint32_t>(i);
			break;
		}
	}

}

void SRVDescriptorHerpManager::DescriptorHeapsMakeNull(uint32_t index)
{

	isNullDescriptorHeaps_[index] = true;

}
