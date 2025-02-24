#include "ClothGPUCollisionManager.h"
#include "../../base/CompileShader.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/DxCommon/DirectXCommon.h"
#include "../../base/BufferResource.h"

void ClothGPUCollisionManager::Initialize()
{

	// デバイス
	device_ = DirectXCommon::GetInstance()->DirectXCommon::GetDevice();

	// バッファ作成関数初期化
	crateBufferFunctions_[kCollisionTypeIndexPlane] = std::bind(&ClothGPUCollisionManager::CreateBufferPlane, this, std::placeholders::_1);
	crateBufferFunctions_[kCollisionTypeIndexSphere] = std::bind(&ClothGPUCollisionManager::CreateBufferSphere, this, std::placeholders::_1);
	crateBufferFunctions_[kCollisionTypeIndexCapsule] = std::bind(&ClothGPUCollisionManager::CreateBufferCapsule, this, std::placeholders::_1);

	// 衝突確認関数初期化
	collisionFunctions_[kCollisionTypeIndexPlane] = std::bind(&ClothGPUCollisionManager::PlaneExecution, this
		, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	collisionFunctions_[kCollisionTypeIndexSphere] = std::bind(&ClothGPUCollisionManager::SphereExecution, this
		, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	collisionFunctions_[kCollisionTypeIndexCapsule] = std::bind(&ClothGPUCollisionManager::CapsuleExecution, this
		, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

	// CS初期化
	CSInitializePlane();
	CSInitializeSphere();
	CSInitializeCapsule();

}

void ClothGPUCollisionManager::CrateBuffer(ClothGPUCollision* collision, CollisionTypeIndex index)
{
	crateBufferFunctions_[index](collision);
}

void ClothGPUCollisionManager::Collision(ClothGPUCollision* collision, CollisionTypeIndex index, ID3D12GraphicsCommandList* commandList, D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU, ID3D12Resource* numsBuffer, uint32_t dispatchNum)
{
	collisionFunctions_[index](
		commandList,
		collision,
		massPointIndexSrvHandleGPU,
		numsBuffer,
		dispatchNum);
}

void ClothGPUCollisionManager::CreateBufferPlane(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->SetCollisionDataBuff(BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Plane) + 0xff) & ~0xff));
	myData->GetCollisionDataBuff()->Map(0, nullptr, reinterpret_cast<void**>(myData->GetCollisionDataMapAddress()));
	// 衝突形状データ作成
	ClothGPUCollision::Plane plane{};
	plane.distance = 0.0f;
	plane.normal = { 0.0f, 1.0f, 0.0f };
	// 衝突するデータマップ
	ClothGPUCollision::CollisionDataMap map;
	map = plane;
	myData->GetCollisionDataMap()->swap(map);

}

void ClothGPUCollisionManager::CreateBufferSphere(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->SetCollisionDataBuff(BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Sphere) + 0xff) & ~0xff));
	myData->GetCollisionDataBuff()->Map(0, nullptr, reinterpret_cast<void**>(myData->GetCollisionDataMapAddress()));
	// 衝突形状データ作成
	ClothGPUCollision::Sphere sphere{};
	sphere.radius = 0.5f;
	sphere.position = { 0.0f, 0.0f, 0.0f };
	// 衝突するデータマップ
	ClothGPUCollision::CollisionDataMap map;
	map = sphere;
	myData->GetCollisionDataMap()->swap(map);

}

void ClothGPUCollisionManager::CreateBufferCapsule(ClothGPUCollision* myData)
{

	// 衝突するデータバッファ CBV
	myData->SetCollisionDataBuff(BufferResource::CreateBufferResource(device_, (sizeof(ClothGPUCollision::Capsule) + 0xff) & ~0xff));
	myData->GetCollisionDataBuff()->Map(0, nullptr, reinterpret_cast<void**>(myData->GetCollisionDataMapAddress()));
	// 衝突形状データ作成
	ClothGPUCollision::Capsule capsule{};
	capsule.radius = 0.5f;
	capsule.origin = { 0.0f, 0.0f, 0.0f };
	capsule.diff = { 0.0f, 1.0f, 0.0f };
	// 衝突するデータマップ
	ClothGPUCollision::CollisionDataMap map;
	map = capsule;
	myData->GetCollisionDataMap()->swap(map);

}

void ClothGPUCollisionManager::CSInitializePlane()
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

void ClothGPUCollisionManager::CSInitializeSphere()
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

void ClothGPUCollisionManager::CSInitializeCapsule()
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

void ClothGPUCollisionManager::PlaneExecution(
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

	commandList->SetComputeRootConstantBufferView(2, myData->GetCollisionDataBuff()->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}

void ClothGPUCollisionManager::SphereExecution(
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

	commandList->SetComputeRootConstantBufferView(2, myData->GetCollisionDataBuff()->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}

void ClothGPUCollisionManager::CapsuleExecution(ID3D12GraphicsCommandList* commandList, ClothGPUCollision* myData, D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU, ID3D12Resource* numsBuffer, uint32_t dispatchNum)
{

	commandList->SetPipelineState(pipelineStatesCS_[kCollisionTypeIndexCapsule].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kCollisionTypeIndexCapsule].Get());

	commandList->SetComputeRootDescriptorTable(0, *massPointIndexSrvHandleGPU);

	commandList->SetComputeRootConstantBufferView(1, numsBuffer->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, myData->GetCollisionDataBuff()->GetGPUVirtualAddress());

	commandList->Dispatch((dispatchNum + 1023) / 1024, 1, 1);

}
