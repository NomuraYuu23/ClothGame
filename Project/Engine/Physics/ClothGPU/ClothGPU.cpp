#include "ClothGPU.h"
#include "../../base/BufferResource.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../base/CompileShader.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/Texture/TextureManager.h"
#include "../../Math/DeltaTime.h"
#include "../../2D/ImguiManager.h"

//	平行光源
DirectionalLight* ClothGPU::sDirectionalLight_ = nullptr;
// ポイントライトマネージャ
PointLightManager* ClothGPU::sPointLightManager_ = nullptr;
//	スポットライトマネージャ
SpotLightManager* ClothGPU::sSpotLightManager_ = nullptr;
// 霧マネージャー
FogManager* ClothGPU::sFogManager_ = nullptr;

// ルートシグネチャCS
std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, ClothGPU::kPipelineStateCSIndexOfCount> ClothGPU::rootSignaturesCS_{};
// パイプラインステートオブジェクトCS
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, ClothGPU::kPipelineStateCSIndexOfCount> ClothGPU::pipelineStatesCS_{};

void ClothGPU::StaticInitialize(
	ID3D12Device* device,
	FogManager* sFogManager)
{

	// 霧マネージャー
	sFogManager_ = sFogManager;

	PipelineStateCSInitialize(device);

	// 衝突クラス
	ClothGPUCollision::StaticInitialize();

}

void ClothGPU::SetLight(DirectionalLight* sDirectionalLight, PointLightManager* sPointLightManager, SpotLightManager* sSpotLightManager)
{

	//	平行光源
	sDirectionalLight_ = sDirectionalLight;
	// ポイントライトマネージャ
	sPointLightManager_ = sPointLightManager;
	//	スポットライトマネージャ
	sSpotLightManager_ = sSpotLightManager;

}


void ClothGPU::PipelineStateCSInitialize(ID3D12Device* device)
{

	// それぞれCS初期化
	PipelineStateCSInitializeForInitVertex(device);
	PipelineStateCSInitializeForInitMassPoint(device);
	PipelineStateCSInitializeForInitSurface(device);
	PipelineStateCSInitializeForUpdateExternalOperation(device);
	PipelineStateCSInitializeForUpdateIntegral(device);
	PipelineStateCSInitializeForUpdateSpring(device);
	PipelineStateCSInitializeForUpdateSurface(device);
	PipelineStateCSInitializeForUpdateVertex(device);

}

void ClothGPU::PipelineStateCSInitializeForInitVertex(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE massPointDescriptorRange[1] = {};
	massPointDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	massPointDescriptorRange[0].NumDescriptors = 1;//数は一つ
	massPointDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	massPointDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

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
	rootParameters[3].DescriptorTable.pDescriptorRanges = massPointDescriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(massPointDescriptorRange);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexInitVertex,
		L"Resources/Shaders/ClothGPU/ClothInitVertex.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange1[1] = {};
	descriptorRange1[0].BaseShaderRegister = 1;//iから始まる
	descriptorRange1[0].NumDescriptors = 1;//数は一つ
	descriptorRange1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

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
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange1;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange1);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexInitMassPoint,
		L"Resources/Shaders/ClothGPU/ClothInitMassPoint.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForInitSurface(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE massPointDescriptorRange[1] = {};
	massPointDescriptorRange[0].BaseShaderRegister = 1;//iから始まる
	massPointDescriptorRange[0].NumDescriptors = 1;//数は一つ
	massPointDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	massPointDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = massPointDescriptorRange;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(massPointDescriptorRange);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexInitSurface,
		L"Resources/Shaders/ClothGPU/ClothInitSurface.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForUpdateExternalOperation(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange1[1] = {};
	descriptorRange1[0].BaseShaderRegister = 1;//iから始まる
	descriptorRange1[0].NumDescriptors = 1;//数は一つ
	descriptorRange1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	descriptorRange1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange1;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange1);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexUpdateExternalOperation,
		L"Resources/Shaders/ClothGPU/ClothUpdateExternalOperation.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForUpdateIntegral(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[4] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 2;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexUpdateIntegral,
		L"Resources/Shaders/ClothGPU/ClothUpdateIntegral.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForUpdateSpring(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[6] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange1[1] = {};
	descriptorRange1[0].BaseShaderRegister = 1;//iから始まる
	descriptorRange1[0].NumDescriptors = 1;//数は一つ
	descriptorRange1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	descriptorRange1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange2[1] = {};
	descriptorRange2[0].BaseShaderRegister = 2;//iから始まる
	descriptorRange2[0].NumDescriptors = 1;//数は一つ
	descriptorRange2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[2].Descriptor.ShaderRegister = 2;//レジスタ番号indexとバインド

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = descriptorRange1;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange1);//Tableで利用する数

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[5].DescriptorTable.pDescriptorRanges = descriptorRange2;//Tableの中身の配列を指定
	rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange2);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexUpdateSpring,
		L"Resources/Shaders/ClothGPU/ClothUpdateSpring.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForUpdateSurface(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange1[1] = {};
	descriptorRange1[0].BaseShaderRegister = 1;//iから始まる
	descriptorRange1[0].NumDescriptors = 1;//数は一つ
	descriptorRange1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange1;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange1);//Tableで利用する数

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexUpdateSurface,
		L"Resources/Shaders/ClothGPU/ClothUpdateSurface.CS.hlsl");

}

void ClothGPU::PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device)
{

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[6] = {};

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange1[1] = {};
	descriptorRange1[0].BaseShaderRegister = 1;//iから始まる
	descriptorRange1[0].NumDescriptors = 1;//数は一つ
	descriptorRange1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//UAVを使う
	descriptorRange1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange2[1] = {};
	descriptorRange2[0].BaseShaderRegister = 2;//iから始まる
	descriptorRange2[0].NumDescriptors = 1;//数は一つ
	descriptorRange2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	D3D12_DESCRIPTOR_RANGE descriptorRange3[1] = {};
	descriptorRange3[0].BaseShaderRegister = 3;//iから始まる
	descriptorRange3[0].NumDescriptors = 1;//数は一つ
	descriptorRange3[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange3[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号indexとバインド

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange1;//Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange1);//Tableで利用する数

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange2;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange2);//Tableで利用する数

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = descriptorRange3;//Tableの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange3);//Tableで利用する数

	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てで使う
	rootParameters[5].Descriptor.ShaderRegister = 1;//レジスタ番号indexとバインド

	// 共通関数に送る
	PipelineStateCSCommonInitialize(
		device,
		rootParameters,
		_countof(rootParameters),
		kPipelineStateCSIndexUpdateVertex,
		L"Resources/Shaders/ClothGPU/ClothUpdateVertex.CS.hlsl");

}

void ClothGPU::PipelineStateCSCommonInitialize(
	ID3D12Device* device, 
	D3D12_ROOT_PARAMETER* rootParameters,
	uint32_t numParameters,
	PipelineStateCSIndex pipelineStateCSIndex, 
	const std::wstring& filePath)
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootsignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = numParameters; //配列の長さ

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturesCS_[pipelineStateCSIndex]));
	assert(SUCCEEDED(hr));

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		filePath,
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = rootSignaturesCS_[pipelineStateCSIndex].Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStatesCS_[pipelineStateCSIndex]));
	assert(SUCCEEDED(hr));

}

ClothGPU::~ClothGPU()
{

	// ディスクリプタヒープの場所を開ける
	// 頂点
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(vertUavIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(vertSrvIndexDescriptorHeap_);

	// SRV
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(externalSrvIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(massPointIndexSrvIndexDescriptorHeap_);
	for (uint32_t i = 0; i < kClothSpringBufferStructIndexOfCount; ++i) {
		SRVDescriptorHerpManager::DescriptorHeapsMakeNull(clothSpringBufferStructs_[i].srvIndexDescriptorHeap);
	}

	// URV
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(surfaceDataUavIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(massPointUavIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(springIndexUavIndexDescriptorHeap_);

}

void ClothGPU::Initialize(
	ID3D12Device* device, 
	ID3D12GraphicsCommandList* commandList, 
	const Vector2& scale, 
	const Vector2& div,
	const std::string textureName)
{

	// マテリアル初期化
	MaterialInitialize(textureName);

	// 数の初期化
	NumInitialize(device, div);

	// バッファ初期化
	BufferInitialize(device, scale, div);

	// CSでUAV初期化
	InitializeCS(commandList);

	// 衝突データをクリア
	collisionDatas_.clear();

	// 更新するかを一度falseに
	csUpdate_ = false;

	// 初期化移動したか
	didYouMoveInit_ = false;

}

void ClothGPU::Update()
{

	// 外部操作リセット
	if (didYouMoveInit_) {
		for (uint32_t i = 0; i < numsMap_->massPointNum; ++i) {
			externalMap_[i].isMove = 0;
		}
	}
	else {
		didYouMoveInit_ = true;
	}

	// 時間経過
	perFrameMap_->time_ += perFrameMap_->deltaTime_;

	// cs更新する
	csUpdate_ = true;

}

void ClothGPU::Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera)
{

	// nullptrチェック
	assert(commandList);
	assert(sDirectionalLight_);
	assert(sPointLightManager_);
	assert(sSpotLightManager_);
	assert(sFogManager_);

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マップ
	wvpMap_->matrix = camera->GetViewProjectionMatrix();

	// ここでバネの更新
	UpdateCS(commandList);

	// リソースバリア
	ResouseBarrierToNonPixelShader(commandList);

	// パイプライン設定
	commandList->SetPipelineState(GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexClothGPU].Get());//PS0を設定
	commandList->SetGraphicsRootSignature(GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexClothGPU].Get());

	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	// 平行光源
	sDirectionalLight_->Draw(commandList, 1);

	// カメラCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(2, camera->GetWorldPositionBuff()->GetGPUVirtualAddress());

	// ワールドトランスフォーム
	commandList->SetGraphicsRootConstantBufferView(3, wvpBuff_->GetGPUVirtualAddress());

	//テクスチャ 
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, 4, textureHandle_);

	// ポイントライト
	sPointLightManager_->Draw(commandList, 5);
	// スポットライト
	sSpotLightManager_->Draw(commandList, 6);

	// 霧
	commandList->SetGraphicsRootConstantBufferView(7, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 頂点
	commandList->SetGraphicsRootDescriptorTable(8, vertSrvHandleGPU_);

	//描画
	commandList->DrawInstanced(numsMap_->vertexNum, 1, 0, 0);

	// リソースバリア
	ResouseBarrierToUnorderedAccess(commandList);

}

void ClothGPU::ImGuiDraw(const std::string& name)
{

	ImGui::Begin(name.c_str());

	// relaxation_ 1~6
	ImGui::DragInt("バネの更新の反復の回数", &relaxation_, 0.2f, 1, 6);

	//clothCalcDataMap_
	ImGui::DragFloat("質量", &clothCalcDataMap_->mass, 0.01f, 1.0f, 1000.0f);
	ImGui::DragFloat3("重力", &clothCalcDataMap_->gravity.x, 0.01f, -1000.0f, 1000.0f);
	ImGui::DragFloat3("風力", &clothCalcDataMap_->wind.x, 0.01f, -1000.0f, 1000.0f);
	ImGui::DragFloat("抵抗", &clothCalcDataMap_->speedResistance, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("バネの強度", &clothCalcDataMap_->stiffness, 0.1f, 1.0f, 1000.0f);
	ImGui::DragFloat("構成バネ伸び抵抗", &clothCalcDataMap_->structuralShrink, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("構成バネ縮み抵抗", &clothCalcDataMap_->structuralStretch, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("せん断バネ伸び抵抗", &clothCalcDataMap_->shearShrink, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("せん断バネ縮み抵抗", &clothCalcDataMap_->shearStretch, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("曲げバネ伸び抵抗", &clothCalcDataMap_->bendingShrink, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("曲げバネ縮み抵抗", &clothCalcDataMap_->bendingStretch, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("速度制限", &clothCalcDataMap_->velocityLimit, 0.01f, 0.0f,1000.0f);

	ImGui::End();

}

void ClothGPU::NumInitialize(ID3D12Device* device, const Vector2& div)
{

	// 頂点数(CPUのIndexと同じ数)
	numsBuff_ = BufferResource::CreateBufferResource(device, (sizeof(Nums) + 0xff) & ~0xff);
	numsBuff_->Map(0, nullptr, reinterpret_cast<void**>(&numsMap_));

	// 厚みを持たせるための処理
	numsMap_->clothSurfaceVertexNum = static_cast<uint32_t>(div.y) * static_cast<uint32_t>(div.x) * 6;
	numsMap_->vertexNum = numsMap_->clothSurfaceVertexNum * 2;
	// 側面部分
	//numsMap_->vertexNum += (static_cast<uint32_t>(div.y) + static_cast<uint32_t>(div.x)) * 12;
	numsMap_->vertexNum += static_cast<uint32_t>(div.x) * 6;

	// 質点数
	numsMap_->massPointNum = (static_cast<uint32_t>(div.y) + 1) * (static_cast<uint32_t>(div.x) + 1);

	// バネ数
	// 構成バネ
	numsMap_->structuralSpringNums[0] = (static_cast<uint32_t>(div.y) + 1) * (static_cast<uint32_t>(div.x) / 2);
	numsMap_->structuralSpringNums[1] = (static_cast<uint32_t>(div.y) + 1) * (static_cast<uint32_t>(div.x) / 2);
	numsMap_->structuralSpringNums[2] = (static_cast<uint32_t>(div.y) / 2) * (static_cast<uint32_t>(div.x) + 1);
	numsMap_->structuralSpringNums[3] = (static_cast<uint32_t>(div.y) / 2) * (static_cast<uint32_t>(div.x) + 1);

	// 軸の分割数が奇数の場合追加する
	if (static_cast<uint32_t>(div.x) % 2 == 1) {
		numsMap_->structuralSpringNums[0] += static_cast<uint32_t>(div.y) + 1;
	}
	if (static_cast<uint32_t>(div.y) % 2 == 1) {
		numsMap_->structuralSpringNums[2] += static_cast<uint32_t>(div.x) + 1;
	}

	// せん断バネ
	uint32_t shearSpringNum = (static_cast<uint32_t>(div.y) * (static_cast<uint32_t>(div.x) / 2));
	numsMap_->shearSpringNums[0] = shearSpringNum;
	numsMap_->shearSpringNums[1] = shearSpringNum;
	numsMap_->shearSpringNums[2] = shearSpringNum;
	numsMap_->shearSpringNums[3] = shearSpringNum;

	//  Xの分割数が奇数の場合追加する
	if (static_cast<uint32_t>(div.x) % 2 == 1) {
		numsMap_->shearSpringNums[0] += static_cast<uint32_t>(div.y);
		numsMap_->shearSpringNums[2] += static_cast<uint32_t>(div.y);
	}

	// 曲げバネ
	// X
	uint32_t bendingX = static_cast<uint32_t>(div.x) - 1;
	uint32_t base = bendingX / 4;
	uint32_t add = bendingX % 4;

	// 4で割れる分の処理
	numsMap_->bendingSpringNums[0] = (static_cast<uint32_t>(div.y) + 1) * 2 * base;
	numsMap_->bendingSpringNums[1] = (static_cast<uint32_t>(div.y) + 1) * 2 * base;

	// あまりが0以外の場合それぞれ追加する
	if (add == 1 || add == 2) {
		numsMap_->bendingSpringNums[0] += (static_cast<uint32_t>(div.y) + 1) * add;
	}
	else if (add == 3) {
		numsMap_->bendingSpringNums[0] += (static_cast<uint32_t>(div.y) + 1) * 2;
		numsMap_->bendingSpringNums[1] += (static_cast<uint32_t>(div.y) + 1);
	}

	// Y
	uint32_t bendingY = static_cast<uint32_t>(div.y) - 1;
	base = bendingY / 4;
	add = bendingY % 4;

	// 4で割れる分の処理
	numsMap_->bendingSpringNums[2] = (static_cast<uint32_t>(div.x) + 1) * 2 * base;
	numsMap_->bendingSpringNums[3] = (static_cast<uint32_t>(div.x) + 1) * 2 * base;

	// あまりが0以外の場合それぞれ追加する
	if (add == 1 || add == 2) {
		numsMap_->bendingSpringNums[2] += (static_cast<uint32_t>(div.x) + 1) * add;
	}
	else if (add == 3) {
		numsMap_->bendingSpringNums[2] += (static_cast<uint32_t>(div.x) + 1) * 2;
		numsMap_->bendingSpringNums[3] += (static_cast<uint32_t>(div.x) + 1);
	}

	// 面数
	numsMap_->surfaceNum = numsMap_->vertexNum / 6;

	// バネフェーズの反復回数
	relaxation_ = 4;

}

void ClothGPU::MaterialInitialize(const std::string textureName)
{

	// テクスチャハンドル
	textureHandle_ = TextureManager::Load(textureName, DirectXCommon::GetInstance());
	// マテリアル
	material_.reset(Material::Create());
	material_->SetEnableLighting(HalfLambert);

}

void ClothGPU::BufferInitialize(ID3D12Device* device,
	const Vector2& scale,
	const Vector2& div)
{

	// CBV
	CBVInitialize(device, scale, div);

	// SRV
	SRVInitialize(device);

	// バネバッファ
	SpringBufferInitialize(device);

	// 頂点バッファ
	VertexBufferInitialize(device);

	// UAV
	UAVInitialize(device);

}

void ClothGPU::SpringBufferInitialize(ID3D12Device* device)
{

	// 初期化
	clothSpringBufferStructs_[kClothSpringBufferStructIndexStructuralLeftOdd].Initialize(device, numsMap_->structuralSpringNums[0]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexStructuralLeftEven].Initialize(device, numsMap_->structuralSpringNums[1]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexStructuralTopOdd].Initialize(device, numsMap_->structuralSpringNums[2]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexStructuralTopEven].Initialize(device, numsMap_->structuralSpringNums[3]);

	clothSpringBufferStructs_[kClothSpringBufferStructIndexShearLeftOdd].Initialize(device, numsMap_->shearSpringNums[0]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexShearLeftEven].Initialize(device, numsMap_->shearSpringNums[1]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexShearRightOdd].Initialize(device, numsMap_->shearSpringNums[2]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexShearRightEven].Initialize(device, numsMap_->shearSpringNums[3]);

	clothSpringBufferStructs_[kClothSpringBufferStructIndexBendingLeftNotPrime].Initialize(device, numsMap_->bendingSpringNums[0]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexBendingLeftPrime].Initialize(device, numsMap_->bendingSpringNums[1]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexBendingTopNotPrime].Initialize(device, numsMap_->bendingSpringNums[2]);
	clothSpringBufferStructs_[kClothSpringBufferStructIndexBendingTopPrime].Initialize(device, numsMap_->bendingSpringNums[3]);

	// マッピング
	for (uint32_t i = 0; i < kClothSpringBufferStructIndexOfCount; ++i) {
		springInitNextIndexes_[i] = 0;
	}

	for (uint32_t y = 0; y < static_cast<uint32_t>(createDataMap_->div.y) + 1; ++y) {
		for (uint32_t x = 0; x < static_cast<uint32_t>(createDataMap_->div.x) + 1; ++x) {

			// 構成バネ
			SpringGeneration(x, y, -1, 0, kTypeOfSpringStructuralSpring); // 左の質点とつなぐ
			SpringGeneration(x, y, 0, -1, kTypeOfSpringStructuralSpring); // 上の質点とつなぐ

			// せん断バネ
			SpringGeneration(x, y, -1, -1, kTypeOfSpringShearSpring); // 左上の質点とつなぐ
			SpringGeneration(x, y, 1, -1, kTypeOfSpringShearSpring); // 右上の質点とつなぐ

			// 曲げバネ
			SpringGeneration(x, y, -2, 0, kTypeOfSpringBendingSpring); // 左一つ先の質点とつなぐ
			SpringGeneration(x, y, 0, -2, kTypeOfSpringBendingSpring); // 右一つ先の質点とつなぐ

		}
	}

}

void ClothGPU::VertexBufferInitialize(ID3D12Device* device)
{

	// UAVデータ
	vertBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(VertexData) + 0xff) & ~0xff) * numsMap_->vertexNum);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = numsMap_->vertexNum;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = sizeof(VertexData);

	vertUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(vertBuff_.Get(), nullptr, &uavDesc, vertUavHandleCPU_);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numsMap_->vertexNum;
	srvDesc.Buffer.StructureByteStride = sizeof(VertexData);

	vertSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	vertSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	vertSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(vertBuff_.Get(), &srvDesc, vertSrvHandleCPU_);

}

void ClothGPU::UAVInitialize(ID3D12Device* device)
{

	// 面情報
	surfaceDataBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(SurfaceData) + 0xff) & ~0xff) * numsMap_->surfaceNum);

	D3D12_UNORDERED_ACCESS_VIEW_DESC surfaceDataUavDesc{};

	surfaceDataUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	surfaceDataUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	surfaceDataUavDesc.Buffer.FirstElement = 0;
	surfaceDataUavDesc.Buffer.NumElements = numsMap_->surfaceNum;
	surfaceDataUavDesc.Buffer.CounterOffsetInBytes = 0;
	surfaceDataUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	surfaceDataUavDesc.Buffer.StructureByteStride = sizeof(SurfaceData);

	surfaceDataUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	surfaceDataUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	surfaceDataUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(surfaceDataBuff_.Get(), nullptr, &surfaceDataUavDesc, surfaceDataUavHandleCPU_);

	// 質点情報
	massPointBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(ClothMassPoint) + 0xff) & ~0xff) * numsMap_->massPointNum);

	D3D12_UNORDERED_ACCESS_VIEW_DESC massPointUavDesc{};

	massPointUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	massPointUavDesc.Buffer.FirstElement = 0;
	massPointUavDesc.Buffer.NumElements = numsMap_->massPointNum;
	massPointUavDesc.Buffer.CounterOffsetInBytes = 0;
	massPointUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	massPointUavDesc.Buffer.StructureByteStride = sizeof(ClothMassPoint);

	massPointUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	massPointUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	massPointUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(massPointBuff_.Get(), nullptr, &massPointUavDesc, massPointUavHandleCPU_);

	// バネindex
	springIndexBuff_ = BufferResource::CreateBufferResourceUAV(device, ((sizeof(uint32_t) + 0xff) & ~0xff));

	D3D12_UNORDERED_ACCESS_VIEW_DESC springIndexUavDesc{};

	springIndexUavDesc.Format = DXGI_FORMAT_UNKNOWN;
	springIndexUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	springIndexUavDesc.Buffer.FirstElement = 0;
	springIndexUavDesc.Buffer.NumElements = 1;
	springIndexUavDesc.Buffer.CounterOffsetInBytes = 0;
	springIndexUavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	springIndexUavDesc.Buffer.StructureByteStride = sizeof(uint32_t);

	springIndexUavHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	springIndexUavHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	springIndexUavIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateUnorderedAccessView(springIndexBuff_.Get(), nullptr, &springIndexUavDesc, springIndexUavHandleCPU_);

}

void ClothGPU::SRVInitialize(ID3D12Device* device)
{

	// 外部操作
	externalBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(ExternalOperationData) + 0xff) & ~0xff) * numsMap_->massPointNum);
	//書き込むためのアドレスを取得
	externalBuff_->Map(0, nullptr, reinterpret_cast<void**>(&externalMap_));

	for (uint32_t i = 0; i < numsMap_->massPointNum; ++i) {
		externalMap_[i].position = {0.0f,0.0f,0.0f};
		externalMap_[i].isMove = 0;
		if (i < (createDataMap_->div.x + 1.0f)) {
			externalMap_[i].weight = 0.0f;
		}
		else {
			externalMap_[i].weight = 1.0f;
		}
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
	externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	externalSrvDesc.Buffer.FirstElement = 0;
	externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	externalSrvDesc.Buffer.NumElements = numsMap_->massPointNum;
	externalSrvDesc.Buffer.StructureByteStride = sizeof(ExternalOperationData);

	externalSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	externalSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	externalSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(externalBuff_.Get(), &externalSrvDesc, externalSrvHandleCPU_);

	// 頂点 がどこの質点か
	massPointIndexBuff_ = BufferResource::CreateBufferResource(device, ((sizeof(uint32_t) + 0xff) & ~0xff) * numsMap_->vertexNum);
	//書き込むためのアドレスを取得
	massPointIndexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&massPointIndexMap_));

	// 設定（マッピング）
	SetMassPointIndex();

	D3D12_SHADER_RESOURCE_VIEW_DESC massPointIndexSrvDesc{};
	massPointIndexSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	massPointIndexSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	massPointIndexSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	massPointIndexSrvDesc.Buffer.FirstElement = 0;
	massPointIndexSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	massPointIndexSrvDesc.Buffer.NumElements = numsMap_->vertexNum;
	massPointIndexSrvDesc.Buffer.StructureByteStride = sizeof(uint32_t);

	massPointIndexSrvHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	massPointIndexSrvHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	massPointIndexSrvIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(massPointIndexBuff_.Get(), &massPointIndexSrvDesc, massPointIndexSrvHandleCPU_);

}

void ClothGPU::CBVInitialize(
	ID3D12Device* device,
	const Vector2& scale,
	const Vector2& div)
{

	// 作成時データバッファ
	createDataBuff_ = BufferResource::CreateBufferResource(device, (sizeof(CreateData) + 0xff) & ~0xff);
	createDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&createDataMap_));
	//マップ
	createDataMap_->scale = scale;
	createDataMap_->div = div;

	// WVPバッファ
	wvpBuff_ = BufferResource::CreateBufferResource(device, (sizeof(WVP) + 0xff) & ~0xff);
	wvpBuff_->Map(0, nullptr, reinterpret_cast<void**>(&wvpMap_));
	//WVPマップ
	wvpMap_->matrix = Matrix4x4::MakeIdentity4x4();

	// 布計算バッファ
	clothCalcDataBuff_ = BufferResource::CreateBufferResource(device, (sizeof(ClothCalcData) + 0xff) & ~0xff);
	clothCalcDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&clothCalcDataMap_));
	//マップ
	clothCalcDataMap_->mass = 1.0f;// 質点の質量
	clothCalcDataMap_->gravity = { 0.0f, -9.8f, 0.0f};// 重力
	clothCalcDataMap_->wind = { 0.0f, 0.0f, 0.0f };// 風力
	clothCalcDataMap_->stiffness = 100.0f; // 剛性。バネ定数k
	clothCalcDataMap_->speedResistance = 0.0f; // 速度抵抗
	clothCalcDataMap_->structuralShrink = 10.0f; // 構成バネ伸び抵抗
	clothCalcDataMap_->structuralStretch = 10.0f; // 構成バネ縮み抵抗
	clothCalcDataMap_->shearShrink = 10.0f; // せん断バネ伸び抵抗
	clothCalcDataMap_->shearStretch = 10.0f; // せん断バネ縮み抵抗
	clothCalcDataMap_->bendingShrink = 10.0f; // 曲げバネ伸び抵抗
	clothCalcDataMap_->bendingStretch = 10.0f; // 曲げバネ縮み抵抗
	clothCalcDataMap_->velocityLimit = 1000.0f; // 速度制限

	// 時間バッファ
	perFrameBuff_ = BufferResource::CreateBufferResource(device, (sizeof(PerFrame) + 0xff) & ~0xff);
	perFrameBuff_->Map(0, nullptr, reinterpret_cast<void**>(&perFrameMap_));
	// 時間マップ
	perFrameMap_->deltaTime_ = kDeltaTime_;
	perFrameMap_->time_ = 0.0f;

	// 頂点計算データバッファ
	vertexCalcDataBuff_ = BufferResource::CreateBufferResource(device, (sizeof(VertexCalcData) + 0xff) & ~0xff);
	vertexCalcDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertexCalcDataMap_));
	// 頂点計算データマップ
	vertexCalcDataMap_->thickness = 0.05f;

}

void ClothGPU::InitializeCS(ID3D12GraphicsCommandList* commandList)
{

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	InitVertexCS(commandList);

	InitSurfaceCS(commandList);

	InitMassPointCS(commandList);

	//コマンドリストをクローズ、キック

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	HRESULT hr = dxCommon->GetCommadListLoad()->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { dxCommon->GetCommadListLoad() };
	DxCommand::GetCommandQueue()->ExecuteCommandLists(1, commandLists);

	//実行待ち
	//Fenceの値を更新
	dxCommon->SetFenceVal(dxCommon->GetFenceVal() + 1);
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	DxCommand::GetCommandQueue()->Signal(dxCommon->GetFence(), dxCommon->GetFenceVal());

	//Fenceの値が指定したSignal値にたどり着いているが確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (dxCommon->GetFence()->GetCompletedValue() < dxCommon->GetFenceVal()) {
		//FrenceのSignalを持つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		dxCommon->GetFence()->SetEventOnCompletion(dxCommon->GetFenceVal(), fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//実行が完了したので、アロケータとコマンドリストをリセット
	hr = dxCommon->GetCommandAllocatorLoad()->Reset();
	assert(SUCCEEDED(hr));
	hr = dxCommon->GetCommadListLoad()->Reset(dxCommon->GetCommandAllocatorLoad(), nullptr);
	assert(SUCCEEDED(hr));

}

void ClothGPU::InitVertexCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitVertex].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitVertex].Get());

	commandList->SetComputeRootDescriptorTable(0, vertUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, createDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, massPointIndexSrvHandleGPU_);

	commandList->Dispatch((numsMap_->vertexNum + 1023) / 1024, 1, 1);

}

void ClothGPU::InitMassPointCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitMassPoint].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitMassPoint].Get());

	commandList->SetComputeRootDescriptorTable(0, massPointUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, createDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, springIndexUavHandleGPU_);

	commandList->Dispatch((numsMap_->massPointNum + 1023) / 1024, 1, 1);

}

void ClothGPU::InitSurfaceCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexInitSurface].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexInitSurface].Get());

	commandList->SetComputeRootDescriptorTable(0, surfaceDataUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, massPointIndexSrvHandleGPU_);

	commandList->Dispatch((numsMap_->surfaceNum + 1023) / 1024, 1, 1);

}

void ClothGPU::UpdateCS(ID3D12GraphicsCommandList* commandList)
{
	
	// cs更新するか確認
	if (!csUpdate_) {
		return;
	}
	else {
		csUpdate_ = false;
	}

	UpdateExternalOperationCS(commandList);

	UpdateIntegralCS(commandList);

	UpdateSpringCS(commandList);

	UpdateCollisionCS(commandList);

	UpdateSurfaceCS(commandList);

	UpdateVertexCS(commandList);

}

void ClothGPU::UpdateExternalOperationCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdateExternalOperation].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdateExternalOperation].Get());

	commandList->SetComputeRootConstantBufferView(0, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(1, massPointUavHandleGPU_);

	commandList->SetComputeRootDescriptorTable(2, externalSrvHandleGPU_);

	commandList->Dispatch((numsMap_->massPointNum + 1023) / 1024, 1, 1);

	UAVBarrierMassPoint(commandList);

}

void ClothGPU::UpdateIntegralCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdateIntegral].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdateIntegral].Get());

	commandList->SetComputeRootConstantBufferView(0, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(1, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, clothCalcDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, massPointUavHandleGPU_);

	commandList->Dispatch((numsMap_->massPointNum + 1023) / 1024, 1, 1);

	UAVBarrierMassPoint(commandList);

}

void ClothGPU::UpdateSpringCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdateSpring].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdateSpring].Get());

	commandList->SetComputeRootConstantBufferView(0, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(1, perFrameBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootConstantBufferView(2, clothCalcDataBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, massPointUavHandleGPU_);

	commandList->SetComputeRootDescriptorTable(5, springIndexUavHandleGPU_);

	// 繰り返し計算してよりリアルに
	for (int32_t i = 0; i < relaxation_; ++i) {

		for (int32_t j = 0; j < kClothSpringBufferStructIndexOfCount; ++j) {

			commandList->SetComputeRootDescriptorTable(4, clothSpringBufferStructs_[j].srvHandleGPU);

			uint32_t num = 0;

			if (j < 4) {
				num = numsMap_->structuralSpringNums[j % 4];
			}
			else if (j < 8) {
				num = numsMap_->shearSpringNums[j % 4];
			}
			else {
				num = numsMap_->bendingSpringNums[j % 4];
			}

			commandList->Dispatch((num + 1023) / 1024, 1, 1);

			UAVBarrierMassPoint(commandList);
		}

	}

}

void ClothGPU::UpdateCollisionCS(ID3D12GraphicsCommandList* commandList)
{

	for (std::list<std::pair<std::string, std::unique_ptr<ClothGPUCollision>>>::iterator itr = collisionDatas_.begin();
		itr != collisionDatas_.end(); ++itr) {
	
		itr->second->ExecutionCS(
			commandList,
			&massPointUavHandleGPU_,
			numsBuff_.Get(),
			numsMap_->massPointNum
		);

		UAVBarrierMassPoint(commandList);

	}

}

void ClothGPU::UpdateSurfaceCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdateSurface].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdateSurface].Get());

	commandList->SetComputeRootDescriptorTable(0, surfaceDataUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, massPointUavHandleGPU_);

	commandList->Dispatch((numsMap_->surfaceNum + 1023) / 1024, 1, 1);

	UAVBarrierMassPoint(commandList);
}

void ClothGPU::UpdateVertexCS(ID3D12GraphicsCommandList* commandList)
{

	commandList->SetPipelineState(pipelineStatesCS_[kPipelineStateCSIndexUpdateVertex].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[kPipelineStateCSIndexUpdateVertex].Get());

	commandList->SetComputeRootDescriptorTable(0, vertUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(1, numsBuff_->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(2, massPointIndexSrvHandleGPU_);
	
	commandList->SetComputeRootDescriptorTable(3, massPointUavHandleGPU_);
	
	commandList->SetComputeRootDescriptorTable(4, surfaceDataUavHandleGPU_);

	commandList->SetComputeRootConstantBufferView(5, vertexCalcDataBuff_->GetGPUVirtualAddress());

	commandList->Dispatch((numsMap_->vertexNum + 1023) / 1024, 1, 1);

	UAVBarrierVertex(commandList);

}

void ClothGPU::UAVBarrierVertex(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = vertBuff_.Get();
	commandList->ResourceBarrier(1, &barrier);

}

void ClothGPU::UAVBarrierMassPoint(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = massPointBuff_.Get();
	commandList->ResourceBarrier(1, &barrier);

}

void ClothGPU::ResouseBarrierToNonPixelShader(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = vertBuff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);

}

void ClothGPU::ResouseBarrierToUnorderedAccess(ID3D12GraphicsCommandList* commandList)
{

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = vertBuff_.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	commandList->ResourceBarrier(1, &barrier);

}

void ClothGPU::SetWeight(
	uint32_t y,
	uint32_t x,
	bool isWight)
{

	// 質点があるか
	if (y < static_cast<uint32_t>(createDataMap_->div.y) + 1 &&
		x < static_cast<uint32_t>(createDataMap_->div.x) + 1) {

		// index取得
		uint32_t index = y * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x;
		// 重み
		float value = 0.0f;
		if (isWight) {
			value = 1.0f;
		}
		externalMap_[index].weight = value;
	}

}

void ClothGPU::SetPosition(
	uint32_t y,
	uint32_t x,
	const Vector3& position)
{

	// 質点があるか
	if (y < static_cast<uint32_t>(createDataMap_->div.y) + 1 &&
		x < static_cast<uint32_t>(createDataMap_->div.x) + 1) {

		// index取得
		uint32_t index = y * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x;

		// 位置
		externalMap_[index].position = position;
		// フラグを立てる
		externalMap_[index].isMove = 1;
	}

}

void ClothGPU::SpringGeneration(uint32_t x, uint32_t y, int32_t offsetX, int32_t offsetY, TypeOfSpring type)
{

	// 終点位置
	int32_t targetX = x + offsetX;
	int32_t targetY = y + offsetY;

	// 範囲内か確認
	if (targetX >= 0 &&
		targetX < static_cast<int32_t>(createDataMap_->div.x) + 1 &&
		targetY >= 0 &&
		targetY < static_cast<int32_t>(createDataMap_->div.y) + 1) {

		// バネ
		ClothSpring spring;
		// 質点
		uint32_t index = y * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x;
		spring.pointIndex0 = index;
		index = targetY * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + targetX;
		spring.pointIndex1 = index;
		// 自然長

		Vector2 distance =
			Vector2(
				static_cast<float>(offsetX) * createDataMap_->scale.x / createDataMap_->div.x,
				static_cast<float>(offsetY) * createDataMap_->scale.y / createDataMap_->div.y);

		spring.naturalLength = Vector2::Length(distance);
		// バネの種類
		spring.type = type;

		// バネのindex
		uint32_t springIndex = 0;
		
		// 登録
		if (spring.type == kTypeOfSpringStructuralSpring) {
			if (offsetX == -1) {
				// 0
				if (targetX % 2 == 0) {
					springIndex = kClothSpringBufferStructIndexStructuralLeftOdd;
				}
				// 1
				else {
					springIndex = kClothSpringBufferStructIndexStructuralLeftEven;
				}
			}
			else {
				// 2
				if (targetY % 2 == 0) {
					springIndex = kClothSpringBufferStructIndexStructuralTopOdd;
				}
				// 3
				else {
					springIndex = kClothSpringBufferStructIndexStructuralTopEven;
				}
			}
		}
		else if (spring.type == kTypeOfSpringShearSpring) {
			if (offsetX == -1) {
				// 4
				if (targetX % 2 == 0) {
					springIndex = kClothSpringBufferStructIndexShearLeftOdd;
				}
				// 5
				else {
					springIndex = kClothSpringBufferStructIndexShearLeftEven;
				}
			}
			else {
				// 6
				if (targetX % 2 == 0) {
					springIndex = kClothSpringBufferStructIndexShearRightOdd;
				}
				// 7
				else {
					springIndex = kClothSpringBufferStructIndexShearRightEven;
				}
			}
		}
		else {
			if (offsetX == -2) {
				// 8
				if (targetX % 4 <= 1) {
					springIndex = kClothSpringBufferStructIndexBendingLeftNotPrime;
				}
				// 9
				else {
					springIndex = kClothSpringBufferStructIndexBendingLeftPrime;
				}
			}
			else {
				// 10
				if (targetY % 4 <= 1) {
					springIndex = kClothSpringBufferStructIndexBendingTopNotPrime;
				}
				// 11
				else {
					springIndex = kClothSpringBufferStructIndexBendingTopPrime;
				}
			}

		}

		clothSpringBufferStructs_[springIndex].map[springInitNextIndexes_[springIndex]] = spring;
		springInitNextIndexes_[springIndex]++;

	}

}

void ClothGPU::MaterialUpdate(
	const EulerTransform& uvTransform, 
	const Vector4& color, 
	int enableLighting, 
	float shininess, 
	float environmentCoefficient)
{

	material_->Update(
		uvTransform,
		color,
		enableLighting,
		shininess,
		environmentCoefficient);

}

void ClothGPU::SetMassPointIndex()
{

	uint32_t y = 0;
	uint32_t x = 0;
	// 裏面まで
	for (uint32_t i = 0; i < numsMap_->clothSurfaceVertexNum * 2; ++i) {
		uint32_t mod = i % 6;
		switch (mod)
		{
			// 左上
		case 0:
			massPointIndexMap_[i] = y * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x;
			break;
			// 右上
		case 1:
		case 3:
			massPointIndexMap_[i] = y * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x + 1;
			break;
			// 左下
		case 2:
		case 4:
			massPointIndexMap_[i] = (y + 1) * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x;
			break;
			// 右下
		case 5:
			massPointIndexMap_[i] = (y + 1) * (static_cast<uint32_t>(createDataMap_->div.x) + 1) + x + 1;

			// 一周終了
			x++;
			if (x >= (createDataMap_->div.x)) {
				x = 0;
				y++;
			}

			// 表面終了
			if (numsMap_->clothSurfaceVertexNum - 1 == i) {
				x = 0;
				y = 0;
			}
			break;
		default:
			assert(0);
			break;
		}

	}

	// 側面上部分
	x = 0;
	y = 0;
	uint32_t offsetIndex = numsMap_->clothSurfaceVertexNum * 2;
	for (uint32_t i = 0; i < static_cast<uint32_t>(createDataMap_->div.x) * 6; ++i) {
		massPointIndexMap_[offsetIndex + i] = x + i % 2;
		if (i % 6 == 5) {
			x++;
		}
	}

}

void ClothGPU::CollisionDataRegistration(
	const std::string& name,
	ClothGPUCollision::CollisionTypeIndex collisionType)
{

	// 登録して初期化
	collisionDatas_.emplace_back();
	collisionDatas_.back().first = name;
	collisionDatas_.back().second = std::make_unique<ClothGPUCollision>();
	collisionDatas_.back().second->Initialize(collisionType);

}

void ClothGPU::CollisionDataDelete(const std::string& name)
{

	uint32_t count = 0;
	// 名前が一致したら削除
	for (std::list<std::pair<std::string, std::unique_ptr<ClothGPUCollision>>>::iterator itr = collisionDatas_.begin();
		itr != collisionDatas_.end(); ++itr) {
		if (itr->first == name) {
			break;
		}
		count++;
	}

	// 名前が見つからない
	if (static_cast<uint32_t>(collisionDatas_.size()) == count) {
		return;
	}

	// 削除
	collisionDatas_.erase(std::next(collisionDatas_.begin(), count));

}

void ClothGPU::CollisionDataUpdate(
	const std::string& name,
	ClothGPUCollision::CollisionDataMap& collisionDataMap)
{

	// 名前が一致したら更新
	for (std::list<std::pair<std::string, std::unique_ptr<ClothGPUCollision>>>::iterator itr = collisionDatas_.begin();
		itr != collisionDatas_.end(); ++itr) {

		if (name == itr->first) {
			itr->second->Update(collisionDataMap);
			break;
		}

	}

}

void ClothGPU::ClothSpringBufferStruct::Initialize(ID3D12Device* device, uint32_t num)
{

	// 外部操作
	buff = BufferResource::CreateBufferResource(device, ((sizeof(ClothSpring) + 0xff) & ~0xff) * num);
	//書き込むためのアドレスを取得
	buff->Map(0, nullptr, reinterpret_cast<void**>(&map));

	D3D12_SHADER_RESOURCE_VIEW_DESC externalSrvDesc{};
	externalSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	externalSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	externalSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	externalSrvDesc.Buffer.FirstElement = 0;
	externalSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	externalSrvDesc.Buffer.NumElements = num;
	externalSrvDesc.Buffer.StructureByteStride = sizeof(ClothSpring);

	srvHandleCPU = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	srvHandleGPU = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	srvIndexDescriptorHeap = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();

	device->CreateShaderResourceView(buff.Get(), &externalSrvDesc, srvHandleCPU);

}
