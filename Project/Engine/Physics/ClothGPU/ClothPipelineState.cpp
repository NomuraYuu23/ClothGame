#include "ClothPipelineState.h"
#include "../../base/CompileShader.h"
#include "../../base/DxCommon/Log.h"

void ClothPipelineState::Initialize(ID3D12Device* device)
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

void ClothPipelineState::SetPipelineState(ID3D12GraphicsCommandList* commandList, PipelineStateCSIndex index)
{

	commandList->SetPipelineState(pipelineStatesCS_[index].Get());//PS0を設定
	commandList->SetComputeRootSignature(rootSignaturesCS_[index].Get());

}

void ClothPipelineState::PipelineStateCSInitializeForInitVertex(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForInitSurface(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForUpdateExternalOperation(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForUpdateIntegral(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForUpdateSpring(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForUpdateSurface(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device)
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

void ClothPipelineState::PipelineStateCSCommonInitialize(
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