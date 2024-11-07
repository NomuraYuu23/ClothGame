#pragma once
#include <array>
#include "../../../externals/DirectXTex/d3dx12.h"
#include <string>
#include <unordered_map>
#include <wrl.h>
#include "../../../externals/DirectXTex/DirectXTex.h"

#include "../WinApp.h"
#include "../DxCommon/DirectXCommon.h"

using namespace DirectX;

class SRVDescriptorHerpManager
{

public: // 静的変数

	//ディスクリプタの数
	static const size_t kNumDescriptors = 2048;
	// ディスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

private: // 静的変数

	// デバイス
	static ID3D12Device* device_;
	// 次のディスクリプタヒープの場所
	static uint32_t nextIndexDescriptorHeap_;
	// ディスクリプタヒープのどこが空いているか
	static std::array<bool, kNumDescriptors> isNullDescriptorHeaps_;

public: // 静的関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SRVDescriptorHerpManager* GetInstance();

	/// <summary>
	///  初期化
	/// </summary>
	/// <param name="device"></param>
	static void Initialize(ID3D12Device* device);

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();

	static void NextIndexDescriptorHeapChange();
	
	static void DescriptorHeapsMakeNull(uint32_t index);

	static uint32_t GetNextIndexDescriptorHeap() { return nextIndexDescriptorHeap_; }

private: // シングルトン

	SRVDescriptorHerpManager() = default;
	~SRVDescriptorHerpManager() = default;
	SRVDescriptorHerpManager(const SRVDescriptorHerpManager&) = delete;
	SRVDescriptorHerpManager& operator=(const SRVDescriptorHerpManager&) = delete;

};

