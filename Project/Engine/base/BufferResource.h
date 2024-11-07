#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
class BufferResource
{

public:
	//Resource作成関数化
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, const size_t& sizeInBytes);

	//Resource作成関数化 UAV版
	static ID3D12Resource* CreateBufferResourceUAV(ID3D12Device* device, const size_t& sizeInBytes);

};

