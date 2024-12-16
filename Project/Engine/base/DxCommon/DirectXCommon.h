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

// クラス
#include "DXGIDevice.h"
#include "SwapChain.h"
#include "DxCommand.h"
#include "../Texture/RenderTargetTexture.h"

/// <summary>
/// DirectX汎用
/// </summary>
class DirectXCommon
{

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">ウインドウアプリケーション</param>
	/// <param name="backBufferWidth">幅</param>
	/// <param name="backBufferHeight">高さ</param>
	void Initialize(
		WinApp* winApp, int32_t backBufferWidth = WinApp::kWindowWidth_,
		int32_t backBufferHeight = WinApp::kWindowHeight_);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

public: // アクセッサ

	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() const { return dxgiDevice_->GetDevice(); }

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommadList() const { return command_->GetCommadList(); }

	/// <summary>
	/// コマンドリストロード取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommadListLoad() const { return loadCommand_->GetCommadList(); }

	/// <summary>
	/// コマンドリストアロケータ取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocator() const { return command_->GetCommandAllocator(); }

	/// <summary>
	/// コマンドリストロードアロケータ取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocatorLoad() const { return loadCommand_->GetCommandAllocator(); }

	/// <summary>
	/// コマンドキュー取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() const { return DxCommand::GetCommandQueue(); }

	/// <summary>
	/// フェンス取得
	/// </summary>
	/// <returns></returns>
	ID3D12Fence* GetFence() const { return fence_.Get(); }

	/// <summary>
	/// フェンス値取得
	/// </summary>
	/// <returns></returns>
	UINT64 GetFenceVal() const { return fenceVal_; }

	/// <summary>
	/// フェンス値設定
	/// </summary>
	/// <param name="fenceVal"></param>
	void SetFenceVal(UINT64 fenceVal) { fenceVal_ = fenceVal; }

	/// <summary>
	/// 幅
	/// </summary>
	/// <returns></returns>
	int32_t GetBackBufferWidth() const { return backBufferWidth_; }

	/// <summary>
	/// 高さ
	/// </summary>
	/// <returns></returns>
	int32_t GetBackBufferHeight() const { return backBufferHeight_; }

	/// <summary>
	/// レンダーターゲットテクスチャ
	/// </summary>
	/// <returns></returns>
	RenderTargetTexture* GetRenderTargetTexture() { return renderTargetTexture_.get(); }

private: // メンバ変数

	// ウィンドウアプリケーション
	WinApp* winApp_;

	// デバイス
	DXGIDevice* dxgiDevice_;
	// スワップチェーン
	SwapChain* swapChain_;

	// コマンド
	std::unique_ptr<DxCommand> command_;
	// 読み込み用コマンド
	std::unique_ptr<DxCommand> loadCommand_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	// フェンス値
	UINT64 fenceVal_ = 0;
	// 幅
	int32_t backBufferWidth_ = 0;
	// 高さ
	int32_t backBufferHeight_ = 0;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	// レンダーターゲット
	std::unique_ptr<RenderTargetTexture> renderTargetTexture_;

	// デバッグレイヤー
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_;

private: // シングルトン
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

private: // メンバ関数

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// デバッグレイヤー
	/// </summary>
	void DebugLayer();

};

