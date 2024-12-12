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

/// <summary>
/// ディスクリプタヒープマネージャーDSV
/// </summary>
class DSVDescriptorHerpManager
{

public: // 静的メンバ変数

	//ディスクリプタの数
	static const size_t kNumDescriptors_ = 16;
	// ディスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device_;
	// 次のディスクリプタヒープの場所
	static uint32_t nextIndexDescriptorHeap_;
	// ディスクリプタヒープのどこが空いているか
	static std::array<bool, kNumDescriptors_> isNullDescriptorHeaps_;

public: // 静的メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DSVDescriptorHerpManager* GetInstance();

	/// <summary>
	///  初期化
	/// </summary>
	/// <param name="device"></param>
	static void Initialize(ID3D12Device* device);

	/// <summary>
	/// CPUハンドル取得
	/// </summary>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();

	/// <summary>
	/// 次のディスクリプタヒープの位置を変更
	/// </summary>
	static void NextIndexDescriptorHeapChange();

	/// <summary>
	/// ディスクリプタヒープをNULLにする
	/// </summary>
	/// <param name="index">番号</param>
	static void DescriptorHeapsMakeNull(uint32_t index);

	/// <summary>
	/// 次のディスクリプタヒープの位置取得
	/// </summary>
	/// <returns></returns>
	static uint32_t GetNextIndexDescriptorHeap() { return nextIndexDescriptorHeap_; }

private: // シングルトン

	DSVDescriptorHerpManager() = default;
	~DSVDescriptorHerpManager() = default;
	DSVDescriptorHerpManager(const DSVDescriptorHerpManager&) = delete;
	DSVDescriptorHerpManager& operator=(const DSVDescriptorHerpManager&) = delete;

};

