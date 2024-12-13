#pragma once
#include <cstdint>
#include "PointLightData.h"
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

// 前方宣言
class Model;
class BaseCamera;

/// <summary>
/// ポイントライトマネージャー
/// </summary>
class PointLightManager
{

public: // 静的メンバ定数

	// パーティクル最大数
	static const uint32_t kNumInstanceMax_ = 4;

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* sDevice_;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;

public: // メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PointLightManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// SRVを作る
	/// </summary>
	void SRVCreate();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pointLightDatas">ポイントライトデータ</param>
	void Update(const std::array<PointLightData, PointLightManager::kNumInstanceMax_>& pointLightDatas);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

private: // メンバ変数

	//WVP用のリソースを作る。
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightDataBuff_;
	//書き込むためのアドレスを取得
	PointLightData* pointLightDataMap_{};
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	// ディスクリプタヒープの位置
	uint32_t indexDescriptorHeap_ = 0;

};

