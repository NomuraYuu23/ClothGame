#include "SwapChain.h"
#include <cassert>
#include "../DescriptorHerpManager/RTVDescriptorHerpManager.h"
#include "../DescriptorHerpManager/DSVDescriptorHerpManager.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include "../BufferResource.h"
#include "../Texture/TextureManager.h"
#include "../../2D/ImguiManager.h"

SwapChain* SwapChain::GetInstance()
{
	static SwapChain instance;
	return &instance;
}

void SwapChain::Initialize(
	int32_t backBufferWidth,
	int32_t backBufferHeight,
	DXGIDevice* dxgiDevice,
	ID3D12CommandQueue* commandQueue,
	WinApp* winApp)
{

	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	//スワップチェーンを生成する
	swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = backBufferWidth_;//画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = backBufferHeight_;//画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用する
	swapChainDesc.BufferCount = kSwapChainNum_;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタにうつしたら、中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiDevice->GetFactory()->CreateSwapChainForHwnd(commandQueue, winApp->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// RTVリソース
	for (uint32_t i = 0; i < kSwapChainNum_; i++)
	{
		
		//SwapChainからResourceを引っ張ってくる
		rtvResources_[i] = { nullptr };
		hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(&rtvResources_[i]));
		//うまく取得できなければ起動できない
		assert(SUCCEEDED(hr));

		//RTVの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに変換して書き込む
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dのテクスチャとして書き込む

		//RTVを作成するDescriptorHeapの場所を決める
		rtvHandles_[i] = RTVDescriptorHerpManager::GetCPUDescriptorHandle();
		rtvIndexDescriptorHeaps_[i] = RTVDescriptorHerpManager::GetNextIndexDescriptorHeap();
		RTVDescriptorHerpManager::NextIndexDescriptorHeapChange();
		//SRVの生成
		dxgiDevice->GetDevice()->CreateRenderTargetView(rtvResources_[i].Get(), &rtvDesc, rtvHandles_[i]);

	}

	// DSVリソース
	//DepthStencilTextureをウィンドウのサイズで作成
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = backBufferWidth_;
	resourceDesc.Height = backBufferHeight_;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Resoureの生成
	hr = dxgiDevice->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&dsvResource_));
	assert(SUCCEEDED(hr));

	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//DSVを作成するDescriptorHeapの場所を決める
	dsvHandle_ = DSVDescriptorHerpManager::GetCPUDescriptorHandle();
	dsvIndexDescriptorHeap_ = DSVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	DSVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	//DSVの生成
	dxgiDevice->GetDevice()->CreateDepthStencilView(dsvResource_.Get(), &dsvDesc, dsvHandle_);

}

void SwapChain::PreDraw(ID3D12GraphicsCommandList* commandList)
{

	assert(commandList);
	commandList_ = commandList;

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = rtvResources_[backBufferIndex].Get();
	//遷移前（現在）のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResoureState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	//描画先のDSVとRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, &dsvHandle_);

	// 全画面クリア
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], clearColor, 0, nullptr);

	// 深度値クリア
	//指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//ビューポート
	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = static_cast<float>(backBufferWidth_);
	viewport.Height = static_cast<float>(backBufferHeight_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	commandList_->RSSetViewports(1, &viewport);//Viewportを設定

	//シザー矩形
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = backBufferWidth_;
	scissorRect.top = 0;
	scissorRect.bottom = backBufferHeight_;
	commandList_->RSSetScissorRects(1, &scissorRect);//Scirssorを設定

}

void SwapChain::PostDraw()
{

	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = rtvResources_[backBufferIndex].Get();

	//画面に描く処理はすべて終わり、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);
	
	commandList_ = nullptr;

}

void SwapChain::Draw(ID3D12GraphicsCommandList* commandList,
	RenderTargetTexture* renderTargetTexture)
{

	PreDraw(commandList);

	renderTargetTexture->TextureDraw(0);

#ifdef _DEMO
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGuiManager::GetInstance()->Draw();
#endif // _DEMO

	PostDraw();

}

