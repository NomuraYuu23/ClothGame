#include "QueryTimestamp.h"
#include <cassert>

#include "../DxCommon/DxCommand.h"
#include "../../2D/ImguiManager.h"

QueryTimestamp* QueryTimestamp::GetInstance()
{
	static QueryTimestamp instance;
	return &instance;
}

void QueryTimestamp::Initialize(ID3D12Device* device)
{

	// クエリ
	D3D12_QUERY_HEAP_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
	desc.Count = kQueryCount * kFrameCount_;
	device->CreateQueryHeap(&desc, IID_PPV_ARGS(&query_));

	// リソース
	D3D12_RESOURCE_DESC resourceDesc;
	memset(&resourceDesc, 0, sizeof(resourceDesc));
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeof(UI64) * kQueryCount * kFrameCount_;
	resourceDesc.Height = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES heap;
	memset(&heap, 0, sizeof(heap));
	heap.Type = D3D12_HEAP_TYPE_READBACK;

	HRESULT hr = device->CreateCommittedResource(
		&heap, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
		IID_PPV_ARGS(&buffer_));
	assert(SUCCEEDED(hr));

	command_ = std::make_unique<DxCommand>();
	command_->Initialize();

	// フェンス
	//初期化0でFenceを作る
	hr = device->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	//FrenceのSignalを持つためのイベントを作成する
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

}

void QueryTimestamp::Preprocessing(ID3D12GraphicsCommandList* commandList)
{

	if (!used_) {
		return;
	}

	// 実行中のクエリを終了
	commandList->EndQuery(
		query_.Get(), 
		D3D12_QUERY_TYPE_TIMESTAMP,
		offset_);

}

void QueryTimestamp::Postprocessing(ID3D12GraphicsCommandList* commandList)
{

	if (!used_) {
		return;
	}

	// 実行中のクエリを終了
	command_->GetCommadList()->EndQuery(
		query_.Get(),
		D3D12_QUERY_TYPE_TIMESTAMP,
		offset_ + 1);

	// クエリデータをバッファに取り出す
	command_->GetCommadList()->ResolveQueryData(
		query_.Get(),
		D3D12_QUERY_TYPE_TIMESTAMP,
		offset_,
		kQueryCount,
		buffer_.Get(),
		offset_ * sizeof(UI64));

	//コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	HRESULT hr = command_->GetCommadList()->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { command_->GetCommadList() };
	DxCommand::GetCommandQueue()->ExecuteCommandLists(1, commandLists);

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


	//次のフレーム用のコマンドリストを準備
	hr = command_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = command_->GetCommadList()->Reset(command_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr));

}

void QueryTimestamp::Reading()
{

	if (!used_) {
		return;
	}

	//GPU タイムスタンプ カウンターがインクリメントされる速度を決定
	UI64 freq = 0;
	DxCommand::GetCommandQueue()->GetTimestampFrequency(&freq);

	// 読みだし
	uint32_t start = (kQueryCount * sizeof(UI64)) * offset_;
	D3D12_RANGE range{ start, start + kQueryCount * sizeof(UI64) };
	void* ptr = nullptr;
	buffer_->Map(0, &range, &ptr);
	UI64* data = reinterpret_cast<UI64*>(ptr);
	time_ = static_cast<float>(data[1] - data[0]) / static_cast<float>(freq) * 1000.0f;
	buffer_->Unmap(0, nullptr);

}

void QueryTimestamp::ImGuiDraw()
{

	if (!used_) {
		return;
	}

	ImGui::Begin("QueryTimestamp");
	ImGui::DragFloat("GPUの処理時間(ms)", &time_,0.0f);
	ImGui::End();

}
