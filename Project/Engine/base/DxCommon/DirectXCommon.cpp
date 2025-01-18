#include "DirectXCommon.h"
#include "../Debug/SafeDelete.h"
#include <algorithm>
#include <cassert>
#include <thread>
#include <timeapi.h>
#include <vector>
#include "../DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../DescriptorHerpManager/RTVDescriptorHerpManager.h"
#include "../DescriptorHerpManager/DSVDescriptorHerpManager.h"
#include "../BufferResource.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include "Log.h"
#include "../GraphicsPipelineState/GraphicsPipelineState.h"
#include "../CompileShader.h"
#include "../Debug/QueryTimestamp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Winmm.lib")

using namespace Microsoft::WRL;

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

// 初期化
void DirectXCommon::Initialize(
	WinApp* winApp, int32_t backBufferWidth,
	int32_t backBufferHeight) {

	// nullptrチェック
	assert(winApp);
	assert(4 <= backBufferWidth && backBufferWidth <= 4096);
	assert(4 <= backBufferHeight && backBufferHeight <= 4096);

	winApp_ = winApp;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

#ifdef _DEBUG
	DebugLayer();
#endif // DEBUG

	// FPS固定初期化
	InitializeFixFPS();

	// DXGIデバイス初期化
	dxgiDevice_ = DXGIDevice::GetInstance();
	dxgiDevice_->Initialize();


	// コンパイルシェーダー
	CompileShader::Initialize(dxgiDevice_->GetDevice());

	// ディスクリプタヒープ初期化
	RTVDescriptorHerpManager::Initialize(dxgiDevice_->GetDevice());
	SRVDescriptorHerpManager::Initialize(dxgiDevice_->GetDevice());
	DSVDescriptorHerpManager::Initialize(dxgiDevice_->GetDevice());

	// グラフィックスパイプライン初期化
	GraphicsPipelineState::Initialize(dxgiDevice_->GetDevice());

	// コマンド関連初期化
	DxCommand::StaticInitialize(dxgiDevice_->GetDevice());
	command_ = std::make_unique<DxCommand>();
	command_->Initialize();
	loadCommand_ = std::make_unique<DxCommand>();
	loadCommand_->Initialize();

	// スワップチェーンの生成
	swapChain_ = SwapChain::GetInstance();
	swapChain_->Initialize(
		backBufferWidth_,
		backBufferHeight_,
		dxgiDevice_,
		DxCommand::GetCommandQueue(),
		winApp_);

	renderTargetTexture_ = std::make_unique<RenderTargetTexture>();
	renderTargetTexture_->Initialize(
		dxgiDevice_->GetDevice(), 
		backBufferWidth_, 
		backBufferHeight_);

	// フェンス生成
	CreateFence();

}

// 描画前処理
void DirectXCommon::PreDraw() {


#ifdef _DEMO

	QueryTimestamp::GetInstance()->Preprocessing(command_->GetCommadList());

#endif // _DEMO

	renderTargetTexture_->PreDraw(command_->GetCommadList());

}
// 描画後処理
void DirectXCommon::PostDraw() {

	renderTargetTexture_->PostDraw();

	swapChain_->Draw(command_->GetCommadList(), renderTargetTexture_.get());

	//コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	HRESULT hr = command_->GetCommadList()->Close();
	assert(SUCCEEDED(hr));

	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { command_->GetCommadList() };
	DxCommand::GetCommandQueue()->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知する
	swapChain_->Present();

	//Fenceの値を更新
	fenceVal_++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	DxCommand::GetCommandQueue()->Signal(fence_.Get(), fenceVal_);

	//Fenceの値が指定したSignal値にたどり着いているが確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceVal_) {
		//FrenceのSignalを持つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceVal_, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

#ifdef _DEMO

	QueryTimestamp::GetInstance()->Postprocessing(command_->GetCommadList());

#endif // _DEMO
	// FPS固定
	UpdateFixFPS();

#ifdef _DEMO
	
	QueryTimestamp::GetInstance()->Reading();

#endif // _DEMO

	//次のフレーム用のコマンドリストを準備
	hr = command_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = command_->GetCommadList()->Reset(command_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr));

}

// フェンス生成
void DirectXCommon::CreateFence() {

	//初期化0でFenceを作る
	HRESULT hr = dxgiDevice_->GetDevice()->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	// リリース版警告回避
	hr;
	assert(SUCCEEDED(hr));

	//FrenceのSignalを持つためのイベントを作成する
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// リリース版警告回避
	fenceEvent;
	// フェンスイベント確認
	assert(fenceEvent != nullptr);

}

void DirectXCommon::InitializeFixFPS()
{

	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();

}

void DirectXCommon::UpdateFixFPS()
{

	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずか
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)経ってない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリーブを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 1マイクロ秒スリーブ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();

}

void DirectXCommon::DebugLayer()
{

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {

		// デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		// GPU
		debugController_->SetEnableGPUBasedValidation(TRUE);

	}

}

