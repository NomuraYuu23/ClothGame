#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <cstdint>
#include "../WinApp.h"
#include "../DxCommon/DXGIDevice.h"
#include "../Texture/RenderTargetTexture.h"

#include "../../2D/SpriteVertex.h"
#include "../GraphicsPipelineState/GraphicsPipelineState.h"

/// <summary>
/// スワップチェイン
/// </summary>
class SwapChain
{

public: // メンバ関数
	
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static SwapChain* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="backBufferWidth">幅</param>
	/// <param name="backBufferHeight">高さ</param>
	/// <param name="dxgiDevice">dxgiDevice</param>
	/// <param name="commandQueue">コマンドキュー</param>
	/// <param name="winApp">ウィンドウアプリケーション</param>
	void Initialize(
		int32_t backBufferWidth,
		int32_t backBufferHeight,
		DXGIDevice* dxgiDevice,
		ID3D12CommandQueue * commandQueue,
		WinApp* winApp);

	/// <summary>
	/// スワップチェーン入手
	/// </summary>
	/// <returns></returns>
	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }

	/// <summary>
	/// リソース
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns></returns>
	ID3D12Resource* GetRTVResource(uint32_t index) { rtvResources_[index].Get(); }
	
	/// <summary>
	/// GPUとOSに画面の交換を行うように通知する
	/// </summary>
	void Present() { swapChain_->Present(1, 0); }

	/// <summary>
	///  描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="renderTargetTexture">レンダーターゲットテクスチャ</param>
	void Draw(ID3D12GraphicsCommandList* commandList,
		RenderTargetTexture* renderTargetTexture);

private: // メンバ関数

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private: // メンバ変数

	// スワップチェーンの数
	static const uint32_t kSwapChainNum_ = 2;

	// 幅
	int32_t backBufferWidth_;
	// 高さ
	int32_t backBufferHeight_;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// RTVリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> rtvResources_[kSwapChainNum_];
	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[kSwapChainNum_];
	// RTVのIndex
	uint32_t rtvIndexDescriptorHeaps_[kSwapChainNum_];
	
	// DSVリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> dsvResource_;
	// DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;
	// DSVのIndex
	uint32_t dsvIndexDescriptorHeap_;

	// コマンドリスト
	ID3D12GraphicsCommandList* commandList_;

};

