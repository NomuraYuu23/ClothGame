#pragma once
#include <array>
#include "../../../externals/DirectXTex/d3dx12.h"
#include <string>
#include <unordered_map>
#include <wrl.h>
#include "../../../externals/DirectXTex/DirectXTex.h"

#include "../WinApp.h"
#include "../DxCommon/DirectXCommon.h"
#include "../DescriptorHerpManager/SRVDescriptorHerpManager.h"

using namespace DirectX;

/// <summary>
/// テクスチャUAV
/// </summary>
class TextureUAV
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureUAV();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	void Initialize(
		ID3D12Device* device,
		int32_t width,
		int32_t height);

	/// <summary>
	/// DescriptorTableをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータの番号</param>
	void SetRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex);

	/// <summary>
	/// バリア
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void UavBarrier(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// ピクセルシェーダーリソースに変更
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void ChangePixelShaderResource(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 順序なしアクセスリソースに変更
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void ChangeUnorderedAccessResource(ID3D12GraphicsCommandList* commandList);
	
	/// <summary>
	/// シェーダーリソースビューのハンドル(GPU)取得
	/// </summary>
	/// <returns></returns>
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetUavHandleGPU() { return uavHandleGPU_; }

	/// <summary>
	/// シェーダーリソースビューのハンドル(GPU)取得
	/// </summary>
	/// <returns></returns>
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return srvHandleGPU_; }

private: // メンバ変数
	
	// テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	// シェーダーリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandleCPU_{};
	// シェーダーリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE uavHandleGPU_{};
	//  SRVのインデックス
	uint32_t uavIndexDescriptorHeap_ = 0;

	// シェーダーリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_{};
	// シェーダーリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_{};
	//  SRVのインデックス
	uint32_t srvIndexDescriptorHeap_ = 0;

};

