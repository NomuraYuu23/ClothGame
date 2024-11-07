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
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">WinApp</param>
	/// <param name="backBufferWidth"></param>
	/// <param name="backBufferHeight"></param>
	void Initialize(
		WinApp* winApp, int32_t backBufferWidth = WinApp::kWindowWidth,
		int32_t backBufferHeight = WinApp::kWindowHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	ID3D12Device* GetDevice() const { return dxgiDevice_->GetDevice(); }

	ID3D12GraphicsCommandList* GetCommadList() const { return command_->GetCommadList(); }

	ID3D12GraphicsCommandList* GetCommadListLoad() const { return loadCommand_->GetCommadList(); }

	ID3D12CommandAllocator* GetCommandAllocator() const { return command_->GetCommandAllocator(); }

	ID3D12CommandAllocator* GetCommandAllocatorLoad() const { return loadCommand_->GetCommandAllocator(); }

	ID3D12CommandQueue* GetCommandQueue() const { return DxCommand::GetCommandQueue(); }

	ID3D12Fence* GetFence() const { return fence_.Get(); }

	UINT64 GetFenceVal() const { return fenceVal_; }

	void SetFenceVal(UINT64 fenceVal) { fenceVal_ = fenceVal; }

	int32_t GetBackBufferWidth() const { return backBufferWidth_; }

	int32_t GetBackBufferHeight() const { return backBufferHeight_; }

	RenderTargetTexture* GetRenderTargetTexture() { return renderTargetTexture_.get(); }

private:

	// ウィンドウアプリケーション
	WinApp* winApp_;

	//Direct3D関連
	DXGIDevice* dxgiDevice_; // デバイス
	SwapChain* swapChain_; // スワップチェーン

	std::unique_ptr<DxCommand> command_; // コマンド
	std::unique_ptr<DxCommand> loadCommand_; // 読み込み用コマンド

	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	int32_t backBufferWidth_ = 0;
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

private: // 関数

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

