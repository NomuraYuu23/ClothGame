#include "Mesh.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/CompileShader.h"

// ルートシグネチャCS
Microsoft::WRL::ComPtr<ID3D12RootSignature> Mesh::rootSignaturesCS_ = nullptr;
// パイプラインステートオブジェクトCS
Microsoft::WRL::ComPtr<ID3D12PipelineState> Mesh::pipelineStatesCS_ = nullptr;

Mesh::~Mesh()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(vertIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(influenceIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(animVertUAVIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(animVertSRVIndexDescriptorHeap_);

}

/// <summary>
/// メッシュデータ生成
/// </summary>
void Mesh::CreateMesh(
	ID3D12Device* sDevice, 
	const std::vector<VertexData>& vertices,
	const std::vector<VertexInfluence>& vertexInfluences,
	const std::vector<uint32_t>& indices,
	ID3D12GraphicsCommandList* commandList) {

	// SkinningInformationバッファ
	skinningInformationBuff_ = BufferResource::CreateBufferResource(sDevice, ((sizeof(SkinningInformation) + 0xff) & ~0xff));
	//書き込むためのアドレスを取得
	skinningInformationBuff_->Map(0, nullptr, reinterpret_cast<void**>(&skinningInformationMap_));
	// マップ
	skinningInformationMap_->num = static_cast<int32_t>(vertices.size());
	skinningInformationMap_->isInverse = false;

	// 頂点バッファ
	VertBuffInitialize(sDevice, vertices);

	// インフルエンスバッファ
	VertInfluenceBuffInitialize(sDevice, vertexInfluences);

	// UAVバッファ
	AnimBuffInitialize(sDevice, vertices, commandList);

	// インデックス
	IndexBufferInitialize(sDevice, indices);

}

void Mesh::SetComputeRootDescriptorTableVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex)
{
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, vertHandleGPU_);
}

void Mesh::SetComputeRootDescriptorTableInfluenceHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex)
{
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, influenceHandleGPU_);
}

void Mesh::SetComputeRootDescriptorTableAnimVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex)
{
	ChangeUnordertedAccess(commandList);
	commandList->SetComputeRootDescriptorTable(rootParameterIndex, animVertUAVHandleGPU_);
}

void Mesh::SetGraphicsRootDescriptorTableAnimVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex)
{
	ChangeNonPixelShaderResource(commandList);
	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, animVertSRVHandleGPU_);
}

void Mesh::SetGraphicsRootDescriptorTableIndexHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex)
{
	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, indexHandleGPU_);
}

void Mesh::ChangeNonPixelShaderResource(ID3D12GraphicsCommandList* commandList)
{

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。
	barrier.Transition.pResource = animVertBuff_.Get();
	//遷移前（現在）のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	//遷移後のResoureState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

}

void Mesh::ChangeUnordertedAccess(ID3D12GraphicsCommandList* commandList)
{

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。
	barrier.Transition.pResource = animVertBuff_.Get();
	//遷移前（現在）のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	//遷移後のResoureState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

}

void Mesh::VertBuffInitialize(ID3D12Device* sDevice, const std::vector<VertexData>& vertices)
{

	// 頂点バッファ
	vertBuff_ = BufferResource::CreateBufferResource(sDevice, ((sizeof(VertexData) + 0xff) & ~0xff) * vertices.size());

	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vbView_.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap_));
	//頂点データをリソースにコピー
	std::memcpy(vertMap_, vertices.data(), sizeof(VertexData) * vertices.size());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = static_cast<UINT>(vertices.size());
	srvDesc.Buffer.StructureByteStride = sizeof(VertexData);
	vertHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	sDevice->CreateShaderResourceView(vertBuff_.Get(), &srvDesc, vertHandleCPU_);

}

void Mesh::VertInfluenceBuffInitialize(ID3D12Device* sDevice, const std::vector<VertexInfluence>& vertexInfluences)
{

	// インフルエンスバッファ
	influenceBuff_ = BufferResource::CreateBufferResource(sDevice, ((sizeof(VertexInfluence) + 0xff) & ~0xff) * vertexInfluences.size());

	//書き込むためのアドレスを取得
	influenceBuff_->Map(0, nullptr, reinterpret_cast<void**>(&influenceMap_));
	//頂点データをリソースにコピー
	std::memcpy(influenceMap_, vertexInfluences.data(), sizeof(VertexInfluence) * vertexInfluences.size());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = static_cast<UINT>(vertexInfluences.size());
	srvDesc.Buffer.StructureByteStride = sizeof(VertexInfluence);
	influenceHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	influenceHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	influenceIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	sDevice->CreateShaderResourceView(influenceBuff_.Get(), &srvDesc, influenceHandleCPU_);

}

void Mesh::AnimBuffInitialize(
	ID3D12Device* sDevice,
	const std::vector<VertexData>& vertices,
	ID3D12GraphicsCommandList* commandList)
{

	// UAVデータ
	animVertBuff_ = BufferResource::CreateBufferResourceUAV(sDevice, ((sizeof(VertexData) + 0xff) & ~0xff) * vertices.size());

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = static_cast<UINT>(vertices.size());
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexData);

	animVertUAVHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	animVertUAVHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	animVertUAVIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	sDevice->CreateUnorderedAccessView(animVertBuff_.Get(), nullptr, &uavDesc, animVertUAVHandleCPU_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = static_cast<UINT>(vertices.size());
	srvDesc.Buffer.StructureByteStride = sizeof(VertexData);

	animVertSRVHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	animVertSRVHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	animVertSRVIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	sDevice->CreateShaderResourceView(animVertBuff_.Get(), &srvDesc, animVertSRVHandleCPU_);

	//CS

	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_.Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_.Get());

	commandList->SetComputeRootDescriptorTable(0, animVertUAVHandleGPU_);
	commandList->SetComputeRootConstantBufferView(1, skinningInformationBuff_->GetGPUVirtualAddress());

	commandList->Dispatch(static_cast<UINT>(vertices.size() + 1023) / 1024, 1, 1);

}

void Mesh::IndexBufferInitialize(ID3D12Device* sDevice, const std::vector<uint32_t>& indices)
{

	// インデックスバッファ
	indexBuff_ = BufferResource::CreateBufferResource(sDevice, ((sizeof(uint32_t) + 0xff) & ~0xff) * indices.size());

	// インデックスバッファビュー
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indices.size());
	ibView_.Format = DXGI_FORMAT_R32_UINT;

	// インデックスバッファマップ
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap_));
	std::memcpy(indexMap_, indices.data(), sizeof(uint32_t) * indices.size());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = static_cast<UINT>(indices.size());
	srvDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	indexHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	indexHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	sDevice->CreateShaderResourceView(indexBuff_.Get(), &srvDesc, indexHandleCPU_);

}

void Mesh::PipelineStateCSInitialize(ID3D12Device* device)
{

	HRESULT hr;

#pragma region Initialize

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[2] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号iとバインド

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters); //配列の長さ

	// サンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc[0].MipLODBias = 0.0f;
	samplerDesc[0].MaxAnisotropy = 0;
	samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc[0].MinLOD = 0.0f;
	samplerDesc[0].MaxLOD = 3.402823466e+38f;
	samplerDesc[0].RegisterSpace = 0;
	samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	descriptionRootsignature.pStaticSamplers = samplerDesc;
	descriptionRootsignature.NumStaticSamplers = _countof(samplerDesc);

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/Model/AnimModelInitialize.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_.Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_));
	assert(SUCCEEDED(hr));

#pragma endregion

}

void Mesh::StaticInitialize(ID3D12Device* device)
{

	PipelineStateCSInitialize(device);

}
