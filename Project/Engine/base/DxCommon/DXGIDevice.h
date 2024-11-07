#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class DXGIDevice
{

public: // 関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DXGIDevice * GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ファクトリ入手
	/// </summary>
	/// <returns></returns>
	IDXGIFactory7* GetFactory() { return dxgiFactory_.Get(); }

	/// <summary>
	/// デバイス入手
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

private: // 変数

	// ファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_;

private: // シングルトン
	DXGIDevice() = default;
	~DXGIDevice() = default;
	DXGIDevice(const DXGIDevice&) = delete;
	const DXGIDevice& operator=(const DXGIDevice&) = delete;

};

