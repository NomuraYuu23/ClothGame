#include "GPUParticle.h"
#include "../base/BufferResource.h"
#include "ParticleCS.h"
#include "../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../base/CompileShader.h"
#include "../base/DxCommon/Log.h"
#include "../base/Texture/TextureManager.h"
#include "../Particle/BillBoardMatrix.h"
#include "../Math/DeltaTime.h"

// パーティクルの最大数
const uint32_t GPUParticle::kParticleMax_ = 1024;
// モデルのディレクトリパス
const std::string GPUParticle::kModelDirectoryPath_ = "Resources/Particle/";
// モデルのディレクトリパス
const std::string GPUParticle::kTextureDirectoryPath_ = "Resources/Particle/";
// モデルのファイルの名前
const std::string GPUParticle::kFilename_ = "plane.obj";
// モデル
std::unique_ptr<Model> GPUParticle::model_ = nullptr;

void GPUParticle::StaticInitialzie()
{

	model_.reset(Model::Create(
		kModelDirectoryPath_,
		kFilename_,
		DirectXCommon::GetInstance()));

}

void GPUParticle::Initialize(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList,
	ID3D12RootSignature* rootSignature,
	ID3D12PipelineState* pipelineState)
{

	// CSの初期化
	PipelineStateCSInitialize(device);

	// バッファの初期化
	UAVBufferInitialize(device, commandList);

	// マテリアル関連の初期化
	MaterialInitialize();

	// 描画用ルートシグネチャ設定
	rootSignature_ = rootSignature;

	// 描画用パイプラインステート設定
	pipelineState_ = pipelineState;
	
	// 定数バッファ初期化
	ConstantBufferInitialzie(device);

}

void GPUParticle::Update()
{

	// 時間加算
	emitterMap_->frequencyTime += kDeltaTime_;

	// 射出間隔を上回ったら射出許可を出して時間を調整
	if (emitterMap_->frequency <= emitterMap_->frequencyTime) {
		emitterMap_->frequencyTime -= emitterMap_->frequency;
		emitterMap_->emit = 1;
	}
	// 射出間隔を上回っていないので、射出許可は出ない
	else {
		emitterMap_->emit = 0;
	}

	// 時間経過
	perFrameMap_->time_ += perFrameMap_->deltaTime_;

}

void GPUParticle::Draw(
	ID3D12GraphicsCommandList* commandList,
	BaseCamera& camera)
{

	assert(commandList);

	// GPUParticleViewのマッピング
	GPUParticleViewMapping(camera);

	// エミット
	Emit(commandList);

	// バリア
	UAVBarrier(commandList);

	// 更新
	UpdateCS(commandList);

	// リソースバリア
	ResouseBarrierToNonPixelShader(commandList);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// パイプライン
	commandList->SetPipelineState(pipelineState_);
	// ルートシグネチャ
	commandList->SetGraphicsRootSignature(rootSignature_);

	// 頂点データ
	commandList->IASetVertexBuffers(0, 1, model_->GetMesh()->GetVbView());

	// GPUパーティクル用
	commandList->SetGraphicsRootDescriptorTable(0, srvHandleGPU_);
	// GPUパーティクルのView
	commandList->SetGraphicsRootConstantBufferView(1, gpuParticleViewBuff_->GetGPUVirtualAddress());
	// テクスチャ
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
		commandList,
		2,
		textureHandle_);
	// マテリアル
	commandList->SetGraphicsRootConstantBufferView(3, material_->GetMaterialBuff()->GetGPUVirtualAddress());
	// 描画
	commandList->DrawInstanced(6, kParticleMax_, 0, 0);

	// リソースバリア
	ResouseBarrierToUnorderedAccess(commandList);

}

void GPUParticle::SetEmitter(const EmitterCS& emitter, bool isEmitSet)
{

	// マッピング
	emitterMap_->count = emitter.count;
	emitterMap_->frequency = emitter.frequency;
	emitterMap_->translate = emitter.translate;
	emitterMap_->radius = emitter.radius;

	if (isEmitSet) {
		emitterMap_->emit = emitter.emit;
		emitterMap_->frequencyTime = emitter.frequencyTime;
	}

}

void GPUParticle::PipelineStateCSInitialize(ID3D12Device* device)
{

	PipelineStateCSInitializeForInitialize(device);

	PipelineStateCSInitializeForEmit(device);

	PipelineStateCSInitializeForUpdate(device);

}

void GPUParticle::UAVBufferInitialize(ID3D12Device* device,
	ID3D12GraphicsCommandList* commandList)
{

	// バッファ
	buff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ParticleCS) + 0xff) & ~0xff) * kParticleMax_);

	/// UAV

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = kParticleMax_;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleCS);

	uavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	uavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	uavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(buff_.Get(), nullptr, &uavDesc, uavHandleCPU_);

	/// ここまでUAV

	/// SRV

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kParticleMax_;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleCS);

	srvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(buff_.Get(), &srvDesc, srvHandleCPU_);

	/// ここまでSRV

	// フリーリストインデックスUAVバッファ
	freeListIndexBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(int32_t) + 0xff) & ~0xff));

	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListIndexUavDesc{};

	freeListIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListIndexUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListIndexUavDesc.Buffer.FirstElement = 0;
	freeListIndexUavDesc.Buffer.NumElements = 1;
	freeListIndexUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListIndexUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListIndexUavDesc.Buffer.StructureByteStride = sizeof(int32_t);

	freeListIndexHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	freeListIndexHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	freeListIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(freeListIndexBuff_.Get(), nullptr, &freeListIndexUavDesc, freeListIndexHandleCPU_);


	// フリーリストUAVバッファ
	freeListBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(uint32_t) + 0xff) & ~0xff) * kParticleMax_);

	D3D12_UNORDERED_ACCESS_VIEW_DESC freeListUavDesc{};

	freeListUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	freeListUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	freeListUavDesc.Buffer.FirstElement = 0;
	freeListUavDesc.Buffer.NumElements = kParticleMax_;
	freeListUavDesc.Buffer.CounterOffsetInBytes = 0;
	freeListUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	freeListUavDesc.Buffer.StructureByteStride = sizeof(uint32_t);

	freeListHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	freeListHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	freeListDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(freeListBuff_.Get(), nullptr, &freeListUavDesc, freeListHandleCPU_);

	// CSによる初期化
	InitialzieCS(commandList);

}

void GPUParticle::MaterialInitialize()
{

	material_.reset(Material::Create());

	if (textureFilename_ == "") {
		textureHandle_ = model_->GetTextureHandles()[0];
	}
	else {
		textureHandle_ = TextureManager::Load(kTextureDirectoryPath_ + textureFilename_, DirectXCommon::GetInstance());
	}

}

void GPUParticle::ConstantBufferInitialzie(ID3D12Device* device)
{

	//GPUParticleViewを作る
	gpuParticleViewBuff_ = BufferResource::CreateBufferResource(device, (sizeof(GPUParticleView) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	gpuParticleViewBuff_->Map(0, nullptr, reinterpret_cast<void**>(&gpuParticleViewMap_));

	// マッピング
	gpuParticleViewMap_->billboardMatrix = Matrix4x4::MakeIdentity4x4();
	gpuParticleViewMap_->viewProjection = Matrix4x4::MakeIdentity4x4();

	//エミッタを作る
	emitterBuff_ = BufferResource::CreateBufferResource(device, (sizeof(EmitterCS) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	emitterBuff_->Map(0, nullptr, reinterpret_cast<void**>(&emitterMap_));

	// マッピング
	emitterMap_->count = 10;
	emitterMap_->frequency = 0.5f;
	emitterMap_->frequencyTime = 0.0f;
	emitterMap_->translate = Vector3(0.0f, 0.0f, 0.0f);
	emitterMap_->radius = 1.0f;
	emitterMap_->emit = 0;

	// 時間バッファ
	perFrameBuff_ = BufferResource::CreateBufferResource(device, (sizeof(PerFrame) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	perFrameBuff_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameMap_));

	// 時間マップ
	perFrameMap_->deltaTime_ = kDeltaTime_;
	perFrameMap_->time_ = 0.0f;

}

void GPUParticle::GPUParticleViewMapping(BaseCamera& camera)
{

	// 全軸
	gpuParticleViewMap_->billboardMatrix = BillBoardMatrix::GetBillBoardMatrixAll(camera);

	gpuParticleViewMap_->viewProjection = camera.GetViewProjectionMatrix();

}

void GPUParticle::InitialzieCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitialize].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootDescriptorTable(1, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(2, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void GPUParticle::Emit(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexEmit].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexEmit].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, emitterBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(4, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void GPUParticle::UpdateCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdate].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get());

	commandList->SetComputeRootDescriptorTable(0, uavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, freeListIndexHandleGPU_);

	commandList->SetComputeRootDescriptorTable(3, freeListHandleGPU_);

	commandList->Dispatch(1, 1, 1);

}

void GPUParticle::UAVBarrier(ID3D12GraphicsCommandList* commandList)
{
	
	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = buff_.Get();
	commandList->ResourceBarrier(1, &barrier);

}

void GPUParticle::ResouseBarrierToNonPixelShader(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = buff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);

}

void GPUParticle::ResouseBarrierToUnorderedAccess(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = buff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	commandList->ResourceBarrier(1, &barrier);

}

void GPUParticle::PipelineStateCSInitializeForInitialize(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

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
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ParticleInitialize.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexInitialize].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexInitialize]));
	assert(SUCCEEDED(hr));

}

void GPUParticle::PipelineStateCSInitializeForEmit(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

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
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ParticleEmit.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexEmit].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexEmit]));
	assert(SUCCEEDED(hr));

}

void GPUParticle::PipelineStateCSInitializeForUpdate(ID3D12Device* device)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListIndexDescriptorRange[1] = {};
	freeListIndexDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	freeListIndexDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListIndexDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListIndexDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE freeListDescriptorRange[1] = {};
	freeListDescriptorRange[0].BaseShaderRegister = 2;//iから始まる
	freeListDescriptorRange[0].NumDescriptors = 1;//数は一つ
	freeListDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	freeListDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = freeListIndexDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(freeListIndexDescriptorRange);//Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = freeListDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(freeListDescriptorRange);//Tableで利用する数

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
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/GPUParticle/ParticleUpdate.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kPipelineStateCSIndexUpdate].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kPipelineStateCSIndexUpdate]));
	assert(SUCCEEDED(hr));

}
