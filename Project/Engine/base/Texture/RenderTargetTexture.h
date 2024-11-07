#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include "../../../externals/DirectXTex/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>

#include "../WinApp.h"
#include "../../2D/SpriteVertex.h"

#include "../GraphicsPipelineState/GraphicsPipelineState.h"

class RenderTargetTexture
{

public: // サブクラス

	/// <summary>
	/// リソースステート
	/// </summary>
	enum ResouceStateIndex {
		kResouceStateIndexRenderTarget, // レンダーターゲット
		kResouceStateIndexPixelShaderResource, // ピクセルシェーダリソース
		kResouceStateIndexNonPixelShaderResource, // ピクセルシェーダ以外のリソース
		kResouceStateIndexOfCount // 数を数える
	};

	/// <summary>
	/// depth版 リソースステート
	/// </summary>
	enum DepthTextureResouceStateIndex {
		kDepthTextureResouceStateIndexDepthWrite, // 深度値書き込み
		kDepthTextureResouceStateIndexPixelShaderResource, // ピクセルシェーダリソース
		kDepthTextureResouceStateIndexNonPixelShaderResource, // ピクセルシェーダ以外のリソース
		kDepthTextureResouceStateIndexOfCount // 数を数える
	};

public: 

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device,
		int32_t backBufferWidth,
		int32_t backBufferHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットをクリア
	/// </summary>
	void ClearRenderTarget(uint32_t resourceIndex);

	/// <summary>
	/// 深度値クリア
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// GPUに送る
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートパラメータインデックス</param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex, uint32_t resourceIndex);

	/// <summary>
	/// レンダーターゲットに変更
	/// </summary>
	/// <param name="resourceIndex">リソースインデックス</param>
	void ChangeRenderTarget(uint32_t resourceIndex);

	/// <summary>
	/// ピクセルシェーダーリソースに変更
	/// </summary>
	/// <param name="resourceIndex">リソースインデックス</param>
	void ChangePixelShaderResource(uint32_t resourceIndex);

	/// <summary>
	/// Nonピクセルシェーダーリソースに変更
	/// </summary>
	/// <param name="resourceIndex">リソースインデックス</param>
	void ChangeNonPixelShaderResource(uint32_t resourceIndex);

	/// <summary>
	/// テクスチャ描画
	/// </summary>
	/// <param name="resourceIndex">リソースインデックス</param>
	void TextureDraw(uint32_t resourceIndex);

	/// <summary>
	/// depth版 深度値書き込みリソースに変更
	/// </summary>
	void DepthTextureChangeDepthWriteResource();

	/// <summary>
	/// depth版 ピクセルシェーダーリソースに変更
	/// </summary>
	void DepthTextureChangePixelShaderResource();

	/// <summary>
	/// depth版 Nonピクセルシェーダーリソースに変更
	/// </summary>
	void DepthTextureChangeNonPixelShaderResource();

	/// <summary>
	/// GPUハンドル入手
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetSrvGPUHandle(uint32_t index) { return srvGPUHandles_[index]; }

	/// <summary>
	/// 深度値テクスチャのGPUハンドル入手
	/// </summary>
	/// <returns></returns>
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetDepthSrvGPUHandle() { return depthSrvGPUHandles_; }

private: // 関数

	/// <summary>
	/// 遷移前（現在）のResouceState取得
	/// </summary>
	/// <param name="resourceIndex">リソースインデックス</param>
	/// <returns></returns>
	D3D12_RESOURCE_STATES GetStateBefore(uint32_t resourceIndex);

	/// <summary>
	/// depth版 遷移前（現在）のResouceState取得
	/// </summary>
	/// <returns></returns>
	D3D12_RESOURCE_STATES GetDepthTextureStateBefore();

private: // 変数

	// リソースの数
	static const uint32_t kResourceNum_ = 8;

	// クリアするときの色
	static const Vector4 kClearColor_;

	// 幅
	int32_t backBufferWidth_;
	// 高さ
	int32_t backBufferHeight_;

	// RTV, SRVリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resources_[kResourceNum_];
	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[kResourceNum_];
	// RTVのIndex
	uint32_t rtvIndexDescriptorHeaps_[kResourceNum_];

	// SRVのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCPUHandles_[kResourceNum_];
	// SRVのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandles_[kResourceNum_];
	//  SRVのインデックス
	uint32_t srvIndexDescriptorHeaps_[kResourceNum_];

	// DSVリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> dsvResource_;
	// DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;
	// DSVのIndex
	uint32_t dsvIndexDescriptorHeap_;

	// depthのSRVのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE depthSrvCPUHandles_;
	// depthのSRVのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE depthSrvGPUHandles_;
	// depthのSRVのインデックス
	uint32_t depthSrvIndexDescriptorHeaps_;

	// コマンドリスト
	ID3D12GraphicsCommandList* commandList_;

	// リソースステート
	ResouceStateIndex resouceStates_[kResourceNum_];

	DepthTextureResouceStateIndex depthTextureResouceStateIndex_;

};

