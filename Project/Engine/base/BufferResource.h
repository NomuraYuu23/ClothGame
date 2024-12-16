#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>

/// <summary>
/// バッファリソース
/// </summary>
class BufferResource
{

public: // メンバ関数

	/// <summary>
	/// Resource作成関数化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="sizeInBytes">サイズ</param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, const size_t& sizeInBytes);

	/// <summary>
	/// Resource作成関数化 UAV版
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="sizeInBytes">サイズ</param>
	/// <returns></returns>
	static ID3D12Resource* CreateBufferResourceUAV(ID3D12Device* device, const size_t& sizeInBytes);

};

