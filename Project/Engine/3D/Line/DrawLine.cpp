#include "DrawLine.h"
#include "../../base/WinApp.h"
#include <cassert>
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

using namespace DirectX;
using namespace Microsoft::WRL;

DrawLine* DrawLine::GetInstance()
{

	static DrawLine instance;
	return &instance;

}

void DrawLine::Initialize(ID3D12Device* device, ID3D12RootSignature* rootSignature, ID3D12PipelineState* pipelineState)
{

	assert(device);
	device_ = device;

	rootSignature_ = rootSignature;
	pipelineState_ = pipelineState;

	// lineForGPU用のリソースを作る。
	lineForGPUBuff_ = BufferResource::CreateBufferResource(device_, ((sizeof(LineForGPU) + 0xff) & ~0xff) * kNumInstanceMax_);
	//書き込むためのアドレスを取得
	lineForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&lineForGPUMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		lineForGPUMap_[i].position[0] = { 0.0f,0.0f,0.0f };
		lineForGPUMap_[i].position[1] = { 0.0f,0.0f,0.0f };
		lineForGPUMap_[i].color[0] = { 1.0f,1.0f,1.0f,1.0f };
		lineForGPUMap_[i].color[1] = { 1.0f,1.0f,1.0f,1.0f };
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(LineForGPU);
	lineForGPUHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	lineForGPUHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(lineForGPUBuff_.Get(), &instancingSrvDesc, lineForGPUHandleCPU_);

	numInstance_ = 0;

}

void DrawLine::Draw(
	ID3D12GraphicsCommandList* commandList,
	BaseCamera& camera)
{

	// 表示するものがない
	if (numInstance_ == 0) {
		return;
	}

	assert(commandList_ == nullptr);
	commandList_ = commandList;

	//RootSignatureを設定。
	commandList_->SetPipelineState(pipelineState_);//PS0を設定
	commandList_->SetGraphicsRootSignature(rootSignature_);
	//形状を設定。PS0に設定しているものとは別。
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//VP CBufferの場所を設定
	commandList_->SetGraphicsRootConstantBufferView(0, camera.GetViewProjectionMatrixBuff()->GetGPUVirtualAddress());

	// LineForGPU
	commandList_->SetGraphicsRootDescriptorTable(1, lineForGPUHandleGPU_);

	//描画
	commandList_->DrawInstanced(kVertNum_, numInstance_, 0, 0);

	//コマンドリストを解除
	DrawLine::commandList_ = nullptr;

	// 次フレームのために0に
	numInstance_ = 0;

}

void DrawLine::Map(const LineForGPU& lineForGPU)
{

	assert(numInstance_ != kNumInstanceMax_);

	lineForGPUMap_[numInstance_] = lineForGPU;

	numInstance_++;

}
