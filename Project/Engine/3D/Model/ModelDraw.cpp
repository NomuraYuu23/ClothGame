#include "ModelDraw.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../base/Texture/TextureManager.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/CompileShader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// コマンドリスト
ID3D12GraphicsCommandList* ModelDraw::sCommandList = nullptr;
// ルートシグネチャ
ID3D12RootSignature* ModelDraw::sRootSignature[PipelineStateIndex::kPipelineStateIndexOfCount];
// パイプラインステートオブジェクト
ID3D12PipelineState* ModelDraw::sPipelineState[PipelineStateIndex::kPipelineStateIndexOfCount];
//	平行光源
DirectionalLight* ModelDraw::sDirectionalLight_ = nullptr;
// ポイントライトマネージャ
PointLightManager* ModelDraw::sPointLightManager_ = nullptr;
//	スポットライトマネージャ
SpotLightManager* ModelDraw::sSpotLightManager_ = nullptr;
// 霧マネージャー
FogManager* ModelDraw::sFogManager_ = nullptr;
// 環境マップ(映り込み用テクスチャ)ハンドル
uint32_t ModelDraw::sEnvironmentTextureHandle_ = 1024;

// 現在のパイプライン番号
ModelDraw::PipelineStateIndex ModelDraw::currentPipelineStateIndex_ = kPipelineStateIndexOfCount;

// ルートシグネチャCS
Microsoft::WRL::ComPtr<ID3D12RootSignature> ModelDraw::sRootSignatureCS_ = nullptr;
// パイプラインステートオブジェクトCS
Microsoft::WRL::ComPtr<ID3D12PipelineState> ModelDraw::sPipelineStateCS_ = nullptr;

void ModelDraw::Initialize(
	const std::array<ID3D12RootSignature*, PipelineStateIndex::kPipelineStateIndexOfCount>& rootSignature,
	const std::array<ID3D12PipelineState*, PipelineStateIndex::kPipelineStateIndexOfCount>& pipelineState)
{

	// グラフィックパイプライン生成
	for (uint32_t i = 0u; i < PipelineStateIndex::kPipelineStateIndexOfCount; i++) {
		sRootSignature[i] = rootSignature[i];
		sPipelineState[i] = pipelineState[i];
	}

	HRESULT hr;
	// デバイス取得
	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

#pragma region ルートシグネチャ設定

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	uint32_t rootParametersIndex = 0;

	// 定数バッファ * 1
	rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
	rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //ALLで使う
	rootParameters[rootParametersIndex].Descriptor.ShaderRegister = 0;                  //レジスタ番号iとバインド

	rootParametersIndex++;

	// ディスクリプタレンジ
	std::vector<std::vector<D3D12_DESCRIPTOR_RANGE>> descriptorRanges;
	descriptorRanges.resize(4);

	// SRV * 3
	for (uint32_t i = 0; i < 3; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRanges[i].push_back(descriptorRange[0]);

		rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
		rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
		rootParameters[rootParametersIndex].DescriptorTable.pDescriptorRanges = descriptorRanges[i].data();//Tableの中身の配列を指定
		rootParameters[rootParametersIndex].DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRanges[i].size());//Tableで利用する数

		rootParametersIndex++;

	}

	// UAV * 1
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;//iから始まる
	descriptorRange[0].NumDescriptors = 1;//数は一つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

	descriptorRanges[3].push_back(descriptorRange[0]);

	rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
	rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
	rootParameters[rootParametersIndex].DescriptorTable.pDescriptorRanges = descriptorRanges[3].data();//Tableの中身の配列を指定
	rootParameters[rootParametersIndex].DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRanges[3].size());//Tableで利用する数

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignatureCS_));
	assert(SUCCEEDED(hr));

#pragma endregion

	// シェーダコンパイル
	IDxcBlob* shader = CompileShader::Compile(
		L"Resources/shaders/Model/AnimModel.CS.hlsl",
		L"cs_6_0",
		L"main");

	// パイプライン
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = shader->GetBufferPointer();
	desc.CS.BytecodeLength = shader->GetBufferSize();
	desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.pRootSignature = sRootSignatureCS_.Get();

	hr = device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&sPipelineStateCS_));
	assert(SUCCEEDED(hr));

}

void ModelDraw::PreDraw(const PreDrawDesc& desc)
{

	assert(sCommandList == nullptr);
	assert(desc.directionalLight);
	assert(desc.pointLightManager);
	assert(desc.spotLightManager);
	assert(desc.fogManager);
	assert(desc.environmentTextureHandle != 1024);

	sCommandList = desc.commandList;

	// SRV
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	sCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//形状を設定。PS0に設定しているものとは別。
	sCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	sDirectionalLight_ = desc.directionalLight;
	sPointLightManager_ = desc.pointLightManager;
	sSpotLightManager_ = desc.spotLightManager;
	sFogManager_ = desc.fogManager;
	sEnvironmentTextureHandle_ = desc.environmentTextureHandle;

}

void ModelDraw::PostDraw()
{

	// コマンドリストを解除
	sCommandList = nullptr;

	sDirectionalLight_ = nullptr;
	sPointLightManager_ = nullptr;
	sSpotLightManager_ = nullptr;
	sFogManager_ = nullptr;

	currentPipelineStateIndex_ = kPipelineStateIndexOfCount;

}

void ModelDraw::AnimObjectDraw(AnimObjectDesc& desc, uint32_t renderTargetIndex)
{

	// nullptrチェック
	assert(sCommandList);

	// ワールドトランスフォームマップ処理
	desc.worldTransform->Map(desc.camera->GetViewProjectionMatrix());

	desc.model->GetMesh()->GetSkinningInformationMap()->isInverse = false;
	UpdateVertexUAV(desc.model, desc.localMatrixManager);

	// パイプライン設定
	if (renderTargetIndex == 2) {
		sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexAnimModelRT2]);//PS0を設定
		sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexAnimModelRT2]);
		currentPipelineStateIndex_ = kPipelineStateIndexAnimModelRT2;
	}
	else if (renderTargetIndex == 3) {
		assert(0);
	}
	else {
		sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexAnimModel]);//PS0を設定
		sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexAnimModel]);
		currentPipelineStateIndex_ = kPipelineStateIndexAnimModel;
	}

	//マテリアルCBufferの場所を設定
	if (desc.material) {
		sCommandList->SetGraphicsRootConstantBufferView(0, desc.material->GetMaterialBuff()->GetGPUVirtualAddress());
	}
	else {
		sCommandList->SetGraphicsRootConstantBufferView(0, Model::GetDefaultMaterial()->GetMaterialBuff()->GetGPUVirtualAddress());
	}

	// 平行光源
	sDirectionalLight_->Draw(sCommandList, 1);

	// カメラCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(2, desc.camera->GetWorldPositionBuff()->GetGPUVirtualAddress());

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootConstantBufferView(3, desc.worldTransform->GetTransformationMatrixBuff()->GetGPUVirtualAddress());

	//テクスチャ 4~11
	if (desc.textureHandles.empty()) {
		for (size_t i = 0; i < desc.model->GetModelData().material.textureFilePaths.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.model->GetTextureHandles()[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.model->GetModelData().material.textureFilePaths.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.model->GetModelData().material.textureFilePaths.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}
	else {
		for (size_t i = 0; i < desc.textureHandles.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.textureHandles[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.textureHandles.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.textureHandles.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}

	// ポイントライト
	sPointLightManager_->Draw(sCommandList, 12);
	// スポットライト
	sSpotLightManager_->Draw(sCommandList, 13);

	// 霧
	sCommandList->SetGraphicsRootConstantBufferView(14, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 環境マップ(映り込み用テクスチャ)ハンドル
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 15, sEnvironmentTextureHandle_);

	// 頂点
	desc.model->GetMesh()->SetGraphicsRootDescriptorTableAnimVertHandleGPU(sCommandList, 16);

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), 1, 0, 0);

}

void ModelDraw::NormalObjectDraw(NormalObjectDesc& desc)
{

	// nullptrチェック
	assert(sCommandList);

	// ワールドトランスフォームマップ処理
	desc.worldTransform->Map(desc.camera->GetViewProjectionMatrix());

	// パイプライン設定
	sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexModel]);//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexModel]);
	currentPipelineStateIndex_ = kPipelineStateIndexModel;

	sCommandList->IASetVertexBuffers(0, 1, desc.model->GetMesh()->GetVbView());

	//マテリアルCBufferの場所を設定
	if (desc.material) {
		sCommandList->SetGraphicsRootConstantBufferView(0, desc.material->GetMaterialBuff()->GetGPUVirtualAddress());
	}
	else {
		sCommandList->SetGraphicsRootConstantBufferView(0, Model::GetDefaultMaterial()->GetMaterialBuff()->GetGPUVirtualAddress());
	}

	// 平行光源
	sDirectionalLight_->Draw(sCommandList, 1);

	// カメラCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(2, desc.camera->GetWorldPositionBuff()->GetGPUVirtualAddress());

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootConstantBufferView(3, desc.worldTransform->GetTransformationMatrixBuff()->GetGPUVirtualAddress());

	//テクスチャ 4~11
	if (desc.textureHandles.empty()) {
		for (size_t i = 0; i < desc.model->GetModelData().material.textureFilePaths.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.model->GetTextureHandles()[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.model->GetModelData().material.textureFilePaths.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.model->GetModelData().material.textureFilePaths.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}
	else {
		for (size_t i = 0; i < desc.textureHandles.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.textureHandles[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.textureHandles.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.textureHandles.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}

	// ポイントライト
	sPointLightManager_->Draw(sCommandList, 12);
	// スポットライト
	sSpotLightManager_->Draw(sCommandList, 13);

	// 霧
	sCommandList->SetGraphicsRootConstantBufferView(14, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 環境マップ(映り込み用テクスチャ)ハンドル
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 15, sEnvironmentTextureHandle_);

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), 1, 0, 0);

}

void ModelDraw::AnimInverseObjectDraw(AnimObjectDesc& desc, uint32_t renderTargetIndex)
{

	// nullptrチェック
	assert(sCommandList);

	// ワールドトランスフォームマップ処理
	desc.worldTransform->Map(desc.camera->GetViewProjectionMatrix());

	desc.model->GetMesh()->GetSkinningInformationMap()->isInverse = true;
	UpdateVertexUAV(desc.model, desc.localMatrixManager);

	// パイプライン設定
	if (renderTargetIndex == 2) {
		sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexAnimInverseModelRT2]);//PS0を設定
		sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexAnimInverseModelRT2]);
		currentPipelineStateIndex_ = kPipelineStateIndexAnimInverseModelRT2;
	}
	else if (renderTargetIndex == 3) {
		assert(0);
	}
	else {
		sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexAnimInverseModel]);//PS0を設定
		sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexAnimInverseModel]);
		currentPipelineStateIndex_ = kPipelineStateIndexAnimInverseModel;
	}

	//マテリアルCBufferの場所を設定
	if (desc.material) {
		sCommandList->SetGraphicsRootConstantBufferView(0, desc.material->GetMaterialBuff()->GetGPUVirtualAddress());
	}
	else {
		sCommandList->SetGraphicsRootConstantBufferView(0, Model::GetDefaultMaterial()->GetMaterialBuff()->GetGPUVirtualAddress());
	}

	// 平行光源
	sDirectionalLight_->Draw(sCommandList, 1);

	// カメラCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(2, desc.camera->GetWorldPositionBuff()->GetGPUVirtualAddress());

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootConstantBufferView(3, desc.worldTransform->GetTransformationMatrixBuff()->GetGPUVirtualAddress());

	//テクスチャ 4~11
	if (desc.textureHandles.empty()) {
		for (size_t i = 0; i < desc.model->GetModelData().material.textureFilePaths.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.model->GetTextureHandles()[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.model->GetModelData().material.textureFilePaths.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.model->GetModelData().material.textureFilePaths.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}
	else {
		for (size_t i = 0; i < desc.textureHandles.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 4 + static_cast<UINT>(i), desc.textureHandles[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.textureHandles.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				4 + static_cast<UINT>(i + desc.textureHandles.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}

	// ポイントライト
	sPointLightManager_->Draw(sCommandList, 12);
	// スポットライト
	sSpotLightManager_->Draw(sCommandList, 13);

	// 霧
	sCommandList->SetGraphicsRootConstantBufferView(14, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 環境マップ(映り込み用テクスチャ)ハンドル
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 15, sEnvironmentTextureHandle_);

	// 頂点
	desc.model->GetMesh()->SetGraphicsRootDescriptorTableAnimVertHandleGPU(sCommandList, 16);

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), 1, 0, 0);

}

void ModelDraw::ManyAnimObjectsDraw(ManyAnimObjectsDesc& desc)
{

	// nullptrチェック
	assert(sCommandList);

	desc.model->GetMesh()->GetSkinningInformationMap()->isInverse = false;
	UpdateVertexUAV(desc.model, desc.localMatrixManager);

	// パイプライン設定
	sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexAnimManyObjects]);//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexAnimManyObjects]);
	currentPipelineStateIndex_ = kPipelineStateIndexAnimManyObjects;

	// マテリアル
	sCommandList->SetGraphicsRootDescriptorTable(0, *desc.materialsHandle);

	//テクスチャ 1~8
	if (desc.textureHandles.empty()) {
		for (size_t i = 0; i < desc.model->GetModelData().material.textureFilePaths.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 1 + static_cast<UINT>(i), desc.model->GetTextureHandles()[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.model->GetModelData().material.textureFilePaths.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				1 + static_cast<UINT>(i + desc.model->GetModelData().material.textureFilePaths.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}
	else {
		for (size_t i = 0; i < desc.textureHandles.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 1 + static_cast<UINT>(i), desc.textureHandles[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.textureHandles.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				1 + static_cast<UINT>(i + desc.textureHandles.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}

	// 平行光源
	sDirectionalLight_->Draw(sCommandList, 9);
	// カメラCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(10, desc.camera->GetWorldPositionBuff()->GetGPUVirtualAddress());
	// ポイントライト
	sPointLightManager_->Draw(sCommandList, 11);
	// スポットライト
	sSpotLightManager_->Draw(sCommandList, 12);

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootDescriptorTable(13, *desc.transformationMatrixesHandle);
	// 霧
	sCommandList->SetGraphicsRootConstantBufferView(14, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 環境マップ(映り込み用テクスチャ)ハンドル
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 15, sEnvironmentTextureHandle_);

	// 頂点
	desc.model->GetMesh()->SetGraphicsRootDescriptorTableAnimVertHandleGPU(sCommandList, 16);

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), desc.numInstance, 0, 0);

}

void ModelDraw::ManyNormalObjectsDraw(ManyNormalObjectsDesc& desc) {


	// nullptrチェック
	assert(sCommandList);

	// パイプライン設定
	sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexManyObjects]);//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexManyObjects]);
	currentPipelineStateIndex_ = kPipelineStateIndexManyObjects;

	//VBVを設定 (インフルエンスと合体)
	sCommandList->IASetVertexBuffers(0, 1, desc.model->GetMesh()->GetVbView());

	// マテリアル
	sCommandList->SetGraphicsRootDescriptorTable(0, *desc.materialsHandle);

	//テクスチャ 1~8
	if (desc.textureHandles.empty()) {
		for (size_t i = 0; i < desc.model->GetModelData().material.textureFilePaths.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 1 + static_cast<UINT>(i), desc.model->GetTextureHandles()[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.model->GetModelData().material.textureFilePaths.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				1 + static_cast<UINT>(i + desc.model->GetModelData().material.textureFilePaths.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}
	else {
		for (size_t i = 0; i < desc.textureHandles.size(); ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 1 + static_cast<UINT>(i), desc.textureHandles[i]);
		}
		uint32_t tooMany = 8 - static_cast<uint32_t>(desc.textureHandles.size());
		for (uint32_t i = 0; i < tooMany; ++i) {
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
				sCommandList,
				1 + static_cast<UINT>(i + desc.textureHandles.size()),
				desc.model->GetTextureHandles()[0]);
		}
	}

	// 平行光源
	sDirectionalLight_->Draw(sCommandList, 9);
	// カメラCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(10, desc.camera->GetWorldPositionBuff()->GetGPUVirtualAddress());
	// ポイントライト
	sPointLightManager_->Draw(sCommandList, 11);
	// スポットライト
	sSpotLightManager_->Draw(sCommandList, 12);

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootDescriptorTable(13, *desc.transformationMatrixesHandle);
	// 霧
	sCommandList->SetGraphicsRootConstantBufferView(14, sFogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

	// 環境マップ(映り込み用テクスチャ)ハンドル
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 15, sEnvironmentTextureHandle_);

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), desc.numInstance, 0, 0);

}

void ModelDraw::NormalOutlineDraw(NormalOutlineDesc& desc)
{

	// nullptrチェック
	assert(sCommandList);

	// パイプライン設定
	sCommandList->SetPipelineState(sPipelineState[kPipelineStateIndexNormalOutline]);//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature[kPipelineStateIndexNormalOutline]);
	currentPipelineStateIndex_ = kPipelineStateIndexNormalOutline;

	sCommandList->IASetVertexBuffers(0, 1, desc.model->GetMesh()->GetVbView());

	// ワールドトランスフォーム
	sCommandList->SetGraphicsRootConstantBufferView(0, desc.worldTransform->GetTransformationMatrixBuff()->GetGPUVirtualAddress());
	// アウトライン
	sCommandList->SetGraphicsRootConstantBufferView(1, desc.outline->GetOutlineDataBuff()->GetGPUVirtualAddress());

	//描画
	sCommandList->DrawInstanced(UINT(desc.model->GetModelData().vertices.size()), 1, 0, 0);

}

void ModelDraw::UpdateVertexUAV(
	Model* model,
	LocalMatrixManager* localMatrixManager)
{

	Mesh* mesh = model->GetMesh();

	sCommandList->SetPipelineState(sPipelineStateCS_.Get());//PS0を設定
	sCommandList->SetComputeRootSignature(sRootSignatureCS_.Get());

	sCommandList->SetComputeRootConstantBufferView(0, mesh->GetSkinningInformationBuff()->GetGPUVirtualAddress());
	mesh->SetComputeRootDescriptorTableVertHandleGPU(sCommandList, 1);
	mesh->SetComputeRootDescriptorTableInfluenceHandleGPU(sCommandList, 2);
	sCommandList->SetComputeRootDescriptorTable(3, localMatrixManager->localMatrixesHandleGPU_);
	mesh->SetComputeRootDescriptorTableAnimVertHandleGPU(sCommandList, 4);

	sCommandList->Dispatch(static_cast<UINT>(model->GetModelData().vertices.size() + 1023 ) / 1024, 1, 1);

}
