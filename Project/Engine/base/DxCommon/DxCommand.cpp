#include "DxCommand.h"
#include <cassert>

// コマンドキュー
Microsoft::WRL::ComPtr<ID3D12CommandQueue> DxCommand::commandQueue_ = nullptr;

ID3D12Device* DxCommand::device_ = nullptr;

void DxCommand::StaticInitialize(ID3D12Device* device)
{
	
	assert(device);
	device_ = device;

	//コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = device_->CreateCommandQueue(&commandQueueDesc,
		IID_PPV_ARGS(&commandQueue_));
	// リリース版警告回避
	hr;
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

}

void DxCommand::Initialize()
{

	HRESULT hr;

	//コマンドアロケータを生成する
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	//コマンドリストを生成する
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

}
