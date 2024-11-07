#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class DxCommand
{

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// コマンドキュー取得
	/// </summary>
	/// <returns></returns>
	static ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

private:

	// コマンドキュー
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;

	// デバイス
	static ID3D12Device* device_;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommadList() const { return commandList_.Get(); }

	/// <summary>
	/// コマンドアロケータ取得
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocator() const { return commandAllocator_.Get(); }

private:

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;

};

