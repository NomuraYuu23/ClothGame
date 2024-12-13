#include "PostEffect.h"
#include "../base/BufferResource.h"
#include "../base/DxCommon/Log.h"
#include "../base/CompileShader.h"
#include <fstream>
#include <cmath>
#include "../2D/ImguiManager.h"

PostEffect* PostEffect::GetInstance()
{
	static PostEffect instance;
	return &instance;
}

void PostEffect::Initialize()
{

	// デバイス取得
	device_ = DirectXCommon::GetInstance()->GetDevice();

	textureManager_ = TextureManager::GetInstance();

	commandList_ = nullptr;

	// 定数バッファ作成
	computeParametersBuff_ = BufferResource::CreateBufferResource(device_, ((sizeof(ComputeParameters) + 0xff) & ~0xff));
	computeParametersBuff_->Map(0, nullptr, reinterpret_cast<void**>(&computeParametersMap_));

	// 定数バッファに渡す値の設定
	computeParametersMap_->threadIdOffsetX = 0; // スレッドのオフセットX
	computeParametersMap_->threadIdTotalX = kTextureWidth_; // スレッドの総数X
	computeParametersMap_->threadIdOffsetY = 0; // スレッドのオフセットY
	computeParametersMap_->threadIdTotalY = kTextureHeight_; // スレッドの総数Y
	computeParametersMap_->threadIdOffsetZ = 0; // スレッドのオフセットZ
	computeParametersMap_->threadIdTotalZ = 1; // スレッドの総数Z

	computeParametersMap_->clearColor = { 0.0f, 1.0f, 0.0f, 1.0f }; // クリアするときの色
	computeParametersMap_->threshold = 0.8f; // しきい値

	computeParametersMap_->kernelSize = 7; // カーネルサイズ
	computeParametersMap_->gaussianSigma = 1.0f; // 標準偏差

	computeParametersMap_->time = 0.0f; // 時間

	computeParametersMap_->rShift = { 0.0f,0.0f }; // Rずらし
	computeParametersMap_->gShift = { 0.0f,0.0f }; // Gずらし
	computeParametersMap_->bShift = { 0.0f,0.0f }; // Bずらし

	computeParametersMap_->distortion = 0.0f; // 歪み

	computeParametersMap_->vignetteSize = 16.0f; // ビネットの大きさ
	computeParametersMap_->vignetteChange = 0.8f; // ビネットの変化

	computeParametersMap_->horzGlitchPase = 0.1f; //水平
	computeParametersMap_->vertGlitchPase = 0.1f; //垂直
	computeParametersMap_->glitchStepValue = 0.1f; // グリッチのステップ値

	computeParametersMap_->radialBlurSamples = 8; // ブラーのサンプル回数
	computeParametersMap_->radialBlurCenter = { 0.5f,0.5f }; // 中心座標
	computeParametersMap_->radialBlurStrength = 0.0f; // ブラーの広がる強さ
	computeParametersMap_->radialBlurMask = 0.0f; // 放射状ブラーが適用されないサイズ

	computeParametersMap_->colorLerpT = 1.0f; // 色変える系のLerpT
	computeParametersMap_->colorSize = { 0.3f, 0.3f }; // 色変える系の大きさ
	computeParametersMap_->colorPosition = { 0.5f, 0.5f }; // 色変える系の位置


	computeParametersMap_->flareColor = { 0.7f, 0.7f, 0.7f, 0.7f }; // フレアの色
	computeParametersMap_->flareSize = { 0.3f, 0.3f }; // フレアの大きさ
	computeParametersMap_->flarePosition = { 0.0f, 0.0f }; // フレアの位置

	computeParametersMap_->paraColor = { 0.2f, 0.2f, 0.2f, 0.7f };  // パラの色
	computeParametersMap_->paraSize = { 0.3f, 0.3f };// パラの大きさ
	computeParametersMap_->paraPosition = { 1.0f, 1.0f }; // パラの位置

	computeParametersMap_->projection = Matrix4x4::MakeIdentity4x4(); // プロジェクション行列
	computeParametersMap_->projectionInverse = Matrix4x4::MakeIdentity4x4(); // プロジェクション逆行列

	computeParametersMap_->outlineSigma = 1.0f; // 標準偏差
	computeParametersMap_->maskEdgeColor = { 1.0f,0.4f,0.3f }; // マスクのエッジの色

	computeParametersMap_->maskThreshold = 0.0f; // マスクしきい値

	computeParametersMap_->maskEdgeRangeOfDetection = 0.03f; // マスクのエッジ検出範囲

	computeParametersMap_->hue = 0.3f;
	computeParametersMap_->saturation = 0.0f;
	computeParametersMap_->value = 0.0f;

	computeParametersMap_->executionFlag = 15;

	// ルートシグネチャ
	CreateRootSignature();

	//シェーダー
	CompileShader();

	// パイプライン
	CreatePipline();

	// 編集する画像初期化
	for (uint32_t i = 0; i < kNumEditTexture_; ++i) {
		editTextures_[i] = std::make_unique<TextureUAV>();
		editTextures_[i]->Initialize(
			device_,
			kTextureWidth_,
			kTextureHeight_);
	}

	// デフォルト速度バッファ
	velocity2DManager_ = std::make_unique<Velocity2DManager>();
	velocity2DManager_->Initialize();
	// デフォルト衝撃波パラメータ
	shockWaveManager_ = std::make_unique<ShockWaveManager>();
	shockWaveManager_->Initialize();

	// マスク用の画像の初期化
	MaskTextureHandleManagerInitialize();

}

void PostEffect::ImGuiDraw()
{

	ImGui::Begin("PostEffect");
	ImGui::Text("time %6.2f", computeParametersMap_->time);
	ImGui::DragFloat("threshold", &computeParametersMap_->threshold, 0.01f, 0.0f, 1.0f);
	ImGui::DragInt("kernelSize", &computeParametersMap_->kernelSize, 2, 3, 55);
	ImGui::DragFloat("gaussianSigma", &computeParametersMap_->gaussianSigma, 0.01f, 0.0f);
	ImGui::DragFloat2("rShift", &computeParametersMap_->rShift.x, 0.01f);
	ImGui::DragFloat2("gShift", &computeParametersMap_->gShift.x, 0.01f);
	ImGui::DragFloat2("bShift", &computeParametersMap_->bShift.x, 0.01f);
	ImGui::DragFloat("distortion", &computeParametersMap_->distortion, 0.01f);
	ImGui::DragFloat("vignetteSize", &computeParametersMap_->vignetteSize, 0.01f);
	ImGui::DragFloat("vignetteChange", &computeParametersMap_->vignetteChange, 0.01f);
	ImGui::DragFloat("horzGlitchPase", &computeParametersMap_->horzGlitchPase, 0.01f);
	ImGui::DragFloat("vertGlitchPase", &computeParametersMap_->vertGlitchPase, 0.01f);
	ImGui::DragFloat("glitchStepValue", &computeParametersMap_->glitchStepValue, 0.01f);
	ImGui::DragInt("radialBlurSamples", &computeParametersMap_->radialBlurSamples, 2, 2, 32);
	ImGui::DragFloat2("radialBlurCenter", &computeParametersMap_->radialBlurCenter.x, 0.01f);
	ImGui::DragFloat("radialBlurStrength", &computeParametersMap_->radialBlurStrength, 0.01f);
	ImGui::DragFloat("radialBlurMask", &computeParametersMap_->radialBlurMask, 0.01f);
	ImGui::DragFloat("colorLerpT", &computeParametersMap_->colorLerpT, 0.01f);
	ImGui::DragFloat2("colorSize", &computeParametersMap_->colorSize.x, 0.01f);
	ImGui::DragFloat2("colorPosition", &computeParametersMap_->colorPosition.x, 0.01f);
	ImGui::ColorEdit4("flareColor", &computeParametersMap_->flareColor.x);
	ImGui::DragFloat2("flareSize", &computeParametersMap_->flareSize.x, 0.01f, 0.0f);
	ImGui::DragFloat2("flarePosition", &computeParametersMap_->flarePosition.x, 0.01f);
	ImGui::ColorEdit4("paraColor", &computeParametersMap_->paraColor.x);
	ImGui::DragFloat2("paraSize", &computeParametersMap_->paraSize.x, 0.01f, 0.0f);
	ImGui::DragFloat2("paraPosition", &computeParametersMap_->paraPosition.x, 0.01f);
	ImGui::DragFloat("outlineSigma", &computeParametersMap_->outlineSigma, 0.01f);
	ImGui::ColorEdit3("maskEdgeColor", &computeParametersMap_->maskEdgeColor.x);
	ImGui::DragFloat("maskThreshold", &computeParametersMap_->maskThreshold, 0.01f);
	ImGui::DragFloat("hue", &computeParametersMap_->hue, 0.01f);
	ImGui::DragFloat("saturation", &computeParametersMap_->saturation, 0.01f);
	ImGui::DragFloat("value", &computeParametersMap_->value, 0.01f);
	ImGui::DragFloat("maskEdgeRangeOfDetection", &computeParametersMap_->maskEdgeRangeOfDetection, 0.001f);

	ImGui::DragInt("executionFlag", &computeParametersMap_->executionFlag, 0.1f, 0, 31);

	ImGui::End();

	velocity2DManager_->ImGuiDraw();
	shockWaveManager_->ImGuiDraw();

}

void PostEffect::Execution(
	ID3D12GraphicsCommandList* commandList,
	RenderTargetTexture* renderTargetTexture,
	CommandIndex commandIndex,
	ExecutionAdditionalDesc* executionAdditionalDesc)
{

	// コマンドリスト
	commandList_ = commandList;

	// コマンドリストがヌルならエラー
	assert(commandList_);

	// ルートシグネチャ
	commandList_->SetComputeRootSignature(rootSignature_.Get());

	// ディスパッチ数
	uint32_t x = (kTextureWidth_ + kNumThreadX_ - 1) / kNumThreadX_;
	uint32_t y = (kTextureHeight_ + kNumThreadY_ - 1) / kNumThreadY_;
	uint32_t z = 1;

	//ソース
	for (uint32_t i = 0; i < 8; ++i) {
		renderTargetTexture->ChangeNonPixelShaderResource(i);
	}

	renderTargetTexture->DepthTextureChangeNonPixelShaderResource();

	for (uint32_t i = 0; i < 4; ++i) {

		// 処理が残っているか
		if (CommandDatas_[commandIndex][i] == kPipelineIndexOfCount) {
			break;
		}

		// パイプライン
		commandList_->SetPipelineState(pipelineStates_[CommandDatas_[commandIndex][i]].Get());

		// バッファを送る
		uint32_t rootParameterIndex = 0;
		// 定数パラメータ
		commandList_->SetComputeRootConstantBufferView(rootParameterIndex, computeParametersBuff_->GetGPUVirtualAddress());
		rootParameterIndex++;

		// 速度パラメータj
		for (uint32_t j = 0; j < 4; ++j) {
			if (executionAdditionalDesc) {
				if (executionAdditionalDesc->velocity2DManagers[j]) {
					commandList_->SetComputeRootConstantBufferView(rootParameterIndex, executionAdditionalDesc->velocity2DManagers[j]->GetVelocity2DDataBuff()->GetGPUVirtualAddress());
				}
				else {
					commandList_->SetComputeRootConstantBufferView(rootParameterIndex, velocity2DManager_->GetVelocity2DDataBuff()->GetGPUVirtualAddress());
				}
			}
			else {
				commandList_->SetComputeRootConstantBufferView(rootParameterIndex, velocity2DManager_->GetVelocity2DDataBuff()->GetGPUVirtualAddress());
			}
			rootParameterIndex++;
		}

		// 衝撃波パラメータ
		for (uint32_t j = 0; j < 4; ++j) {
			if (executionAdditionalDesc) {
				if (executionAdditionalDesc->shockWaveManagers[j]) {
					commandList_->SetComputeRootConstantBufferView(rootParameterIndex, executionAdditionalDesc->shockWaveManagers[j]->GetShockWaveDataBuff()->GetGPUVirtualAddress());
				}
				else {
					commandList_->SetComputeRootConstantBufferView(rootParameterIndex, shockWaveManager_->GetShockWaveDataBuff()->GetGPUVirtualAddress());
				}
			}
			else {
				commandList_->SetComputeRootConstantBufferView(rootParameterIndex, shockWaveManager_->GetShockWaveDataBuff()->GetGPUVirtualAddress());
			}
			rootParameterIndex++;
		}

		// ソース
		for (uint32_t j = 0; j < 8; ++j) {
			commandList_->SetComputeRootDescriptorTable(rootParameterIndex, renderTargetTexture->GetSrvGPUHandle(j));
			rootParameterIndex++;
		}

		// 深度値
		commandList_->SetComputeRootDescriptorTable(rootParameterIndex, renderTargetTexture->GetDepthSrvGPUHandle());
		rootParameterIndex++;

		// マスク
		textureManager_->SetComputeRootDescriptorTable(commandList_, rootParameterIndex, useMaskTextureHandle_);
		rootParameterIndex++;

		// 行先
		for (uint32_t j = 0; j < kNumEditTexture_; ++j) {
			editTextures_[j]->SetRootDescriptorTable(commandList_, rootParameterIndex);
			rootParameterIndex++;
		}

		// 実行
		commandList_->Dispatch(x, y, z);

	}

	//ソース
	for (uint32_t i = 0; i < 8; ++i) {
		renderTargetTexture->ChangeRenderTarget(i);
	}
	renderTargetTexture->DepthTextureChangeDepthWriteResource();

	// コマンドリスト
	commandList_ = nullptr;

}

void PostEffect::SetMaskTextureHandleNumber(uint32_t num)
{

	assert(maskTextureHandles_[num] < kMaskTextureIndexOfCount);

	useMaskTextureHandle_ = maskTextureHandles_[num];

}

void PostEffect::MaskTextureHandleManagerInitialize()
{

	for (uint32_t i = 0; i < kMaskTextureIndexOfCount; ++i) {
		maskTextureHandles_[i] = TextureManager::Load(kMaskTextureDirectoryPaths_[i], DirectXCommon::GetInstance());
	}

	useMaskTextureHandle_ = maskTextureHandles_[0];

}

void PostEffect::CreateRootSignature()
{

	HRESULT hr;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// ルートパラメータ
	D3D12_ROOT_PARAMETER rootParameters[21] = {};
	uint32_t rootParametersIndex = 0;

	// 定数バッファ * 1
	// 速度バッファ * 4
	// 衝撃波バッファ * 4
	for (uint32_t i = 0; i < 9; ++i) {
		
		rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
		rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //ALLで使う
		rootParameters[rootParametersIndex].Descriptor.ShaderRegister = i;                  //レジスタ番号iとバインド
		
		rootParametersIndex++;
	
	}

	descriptorRanges_.resize(12);

	// ソース * 8 + 1 + 1
	for (uint32_t i = 0; i < 10; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRanges_[i].push_back(descriptorRange[0]);

		rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
		rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
		rootParameters[rootParametersIndex].DescriptorTable.pDescriptorRanges = descriptorRanges_[i].data();//Tableの中身の配列を指定
		rootParameters[rootParametersIndex].DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRanges_[i].size());//Tableで利用する数

		rootParametersIndex++;
	
	}

	// 行先 * 2

	uint32_t offset = 10;

	for (uint32_t i = 0; i < 2; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRanges_[i + offset].push_back(descriptorRange[0]);

		rootParameters[rootParametersIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
		rootParameters[rootParametersIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てで使う
		rootParameters[rootParametersIndex].DescriptorTable.pDescriptorRanges = descriptorRanges_[i + offset].data();//Tableの中身の配列を指定
		rootParameters[rootParametersIndex].DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRanges_[i + offset].size());//Tableで利用する数

		rootParametersIndex++;

	}

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

	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));

}

void PostEffect::CreateHeaderHLSL()
{
	
	// ファイルを開く
	std::ofstream file("Resources/shaders/PostEffect/PostEffect.hlsli");

	// ファイルがないのでエラー
	assert(file);

	// スレッド数
	file << "#define" << " " << "THREAD_X" << " " << kNumThreadX_ << "\n";
	file << "#define" << " " << "THREAD_Y" << " " << kNumThreadY_ << "\n";
	file << "#define" << " " << "THREAD_Z" << " " << kNumThreadZ_ << "\n";
	file << "#define" << " " << "PI" << " " << 3.14159265 << "\n";

	// ファイルを閉じる
	file.close();

}

void PostEffect::CompileShader()
{

	for (uint32_t i = 0; i < kPipelineIndexOfCount; ++i) {

		// ヘッダを作成
		CreateHeaderHLSL();

		// コンパイル
		shaders_[i] = CompileShader::Compile(
			shaderNames_[i].first,
			L"cs_6_0",
			shaderNames_[i].second);

	}

}

void PostEffect::CreatePipline()
{

	// パイプライン作成
	for (uint32_t i = 0; i < kPipelineIndexOfCount; ++i) {

		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = shaders_[i]->GetBufferPointer();
		desc.CS.BytecodeLength = shaders_[i]->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = rootSignature_.Get();

		HRESULT hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineStates_[i]));
		assert(SUCCEEDED(hr));
	}

}
