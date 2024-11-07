#include "ClothGPUCollision.h"
#include "../../base/BufferResource.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/CompileShader.h"
#include "../../base/DxCommon/DirectXCommon.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

// デバイス
ID3D12Device* ClothGPUCollision::device_ = nullptr;
// バッファ作成関数群
std::array<std::function<void(ClothGPUCollision*)>, ClothGPUCollision::CollisionTypeIndex::kCollisionTypeIndexOfIndex> ClothGPUCollision::crateBufferFunctions_{};
// 衝突確認関数群
std::array<
	std::function<void(ID3D12GraphicsCommandList*, ClothGPUCollision*, D3D12_GPU_DESCRIPTOR_HANDLE*, ID3D12Resource*, uint32_t)>,
	ClothGPUCollision::kCollisionTypeIndexOfIndex> ClothGPUCollision::collisionFunctions_{};
// ルートシグネチャCS
std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, ClothGPUCollision::CollisionTypeIndex::kCollisionTypeIndexOfIndex> ClothGPUCollision::rootSignaturesCS_{};
// パイプラインステートオブジェクトCS
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, ClothGPUCollision::CollisionTypeIndex::kCollisionTypeIndexOfIndex> ClothGPUCollision::pipelineStatesCS_{};

void ClothGPUCollision::StaticInitialize()
{

	// デバイス
	device_ = DirectXCommon::GetInstance()->DirectXCommon::GetDevice();

	// バッファ作成関数初期化
	crateBufferFunctions_[kCollisionTypeIndexPlane] = ClothGPUCollision::CreateBufferPlane;
	crateBufferFunctions_[kCollisionTypeIndexSphere] = ClothGPUCollision::CreateBufferSphere;
	crateBufferFunctions_[kCollisionTypeIndexCapsule] = ClothGPUCollision::CreateBufferCapsule;

	// 衝突確認関数初期化
	collisionFunctions_[kCollisionTypeIndexPlane] = ClothGPUCollision::PlaneExecution;
	collisionFunctions_[kCollisionTypeIndexSphere] = ClothGPUCollision::SphereExecution;
	collisionFunctions_[kCollisionTypeIndexCapsule] = ClothGPUCollision::CapsuleExecution;

	// CS初期化
	CSInitializePlane();
	CSInitializeSphere();
	CSInitializeCapsule();

}

void ClothGPUCollision::CreateBufferPlane(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->collisionDataBuff_ = BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Plane) + 0xff) & ~0xff);
	myData->collisionDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&myData->collisionDataMap_));
	// 衝突形状データ作成
	ClothGPUCollision::Plane plane{};
	plane.distance_ = 0.0f;
	plane.normal_ = { 0.0f, 1.0f, 0.0f };
	// 衝突するデータマップ
	CollisionDataMap map;
	map = plane;
	myData->collisionDataMap_->swap(map);

}

void ClothGPUCollision::CreateBufferSphere(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->collisionDataBuff_ = BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Sphere) + 0xff) & ~0xff);
	myData->collisionDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&myData->collisionDataMap_));
	// 衝突形状データ作成
	ClothGPUCollision::Sphere sphere{};
	sphere.radius_ = 0.5f;
	sphere.position_ = { 0.0f, 0.0f, 0.0f };
	// 衝突するデータマップ
	CollisionDataMap map;
	map = sphere;
	myData->collisionDataMap_->swap(map);

}

void ClothGPUCollision::CreateBufferCapsule(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->collisionDataBuff_ = BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Capsule) + 0xff) & ~0xff);
	myData->collisionDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&myData->collisionDataMap_));
	// 衝突形状データ作成
	ClothGPUCollision::Capsule capsule{};
	capsule.radius_ = 0.5f;
	capsule.origin_ = { 0.0f, 0.0f, 0.0f };
	capsule.diff_ = { 0.0f, 1.0f, 0.0f };
	// 衝突するデータマップ
	CollisionDataMap map;
	map = capsule;
	myData->collisionDataMap_->swap(map);

}

void ClothGPUCollision::CSInitializePlane()
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

	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kCollisionTypeIndexPlane]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/ClothGPU/Collision/ClothCollisionPlane.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kCollisionTypeIndexPlane].Get();

	hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kCollisionTypeIndexPlane]));
	assert(SUCCEEDED(hr));

}

void ClothGPUCollision::CSInitializeSphere()
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

	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kCollisionTypeIndexSphere]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/ClothGPU/Collision/ClothCollisionSphere.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kCollisionTypeIndexSphere].Get();

	hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kCollisionTypeIndexSphere]));
	assert(SUCCEEDED(hr));

}

void ClothGPUCollision::CSInitializeCapsule()
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

	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[kCollisionTypeIndexCapsule]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/ClothGPU/Collision/ClothCollisionCapsule.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[kCollisionTypeIndexCapsule].Get();

	hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[kCollisionTypeIndexCapsule]));
	assert(SUCCEEDED(hr));

}

void ClothGPUCollision::PlaneExecution(
	ID3D12GraphicsCommandList* commandList, 
	ClothGPUCollision* myData, 
	D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
	ID3D12Resource* numsBuffer,
	uint32_t dispatchNum)
{

	commandList->SetPipelineState(pipelineStatesCS_[kCollisionTypeIndexPlane].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kCollisionTypeIndexPlane].Get());

	commandList->SetComputeRootDescriptorTable(0, *massPointIndexSrvHandleGPU);

	commandList->SetComputeRootConstantBufferView(1, numsBuffer->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, myData->collisionDataBuff_->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}

void ClothGPUCollision::SphereExecution(
	ID3D12GraphicsCommandList* commandList, 
	ClothGPUCollision* myData, 
	D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU, 
	ID3D12Resource* numsBuffer, 
	uint32_t dispatchNum)
{

	commandList->SetPipelineState(pipelineStatesCS_[kCollisionTypeIndexSphere].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kCollisionTypeIndexSphere].Get());

	commandList->SetComputeRootDescriptorTable(0, *massPointIndexSrvHandleGPU);

	commandList->SetComputeRootConstantBufferView(1, numsBuffer->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, myData->collisionDataBuff_->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}

void ClothGPUCollision::CapsuleExecution(ID3D12GraphicsCommandList* commandList, ClothGPUCollision* myData, D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU, ID3D12Resource* numsBuffer, uint32_t dispatchNum)
{

	commandList->SetPipelineState(pipelineStatesCS_[kCollisionTypeIndexCapsule].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kCollisionTypeIndexCapsule].Get());

	commandList->SetComputeRootDescriptorTable(0, *massPointIndexSrvHandleGPU);

	commandList->SetComputeRootConstantBufferView(1, numsBuffer->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, myData->collisionDataBuff_->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}

void ClothGPUCollision::Initialize(CollisionTypeIndex collisionType)
{

	// 衝突タイプ
	collisionType_ = collisionType;

	// バッファ作成
	crateBufferFunctions_[collisionType_](this);

}

void ClothGPUCollision::Update(CollisionDataMap& collisionDataMap)
{

	// 型が同じなら更新
	if (collisionDataMap.index() == collisionDataMap_->index()) {
		collisionDataMap_->swap(collisionDataMap);
	}

}

void ClothGPUCollision::ExecutionCS(
	ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
	ID3D12Resource* numsBuffer,
	uint32_t dispatchNum)
{

	collisionFunctions_[collisionType_](
		commandList,
		this,
		massPointIndexSrvHandleGPU,
		numsBuffer,
		dispatchNum);

}
