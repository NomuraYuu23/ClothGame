#include "GraphicsPipelineState.h"
#include "../CompileShader.h"
#include "../DxCommon/Log.h"

using namespace Microsoft::WRL;

// ルートシグネチャ
Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsPipelineState::sRootSignature[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];
// パイプラインステートオブジェクト
Microsoft::WRL::ComPtr<ID3D12PipelineState> GraphicsPipelineState::sPipelineState[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];
// デバイス
ID3D12Device* GraphicsPipelineState::sDevice_;

void GraphicsPipelineState::Initialize(ID3D12Device* sDevice)
{

	// デバイス
	sDevice_ = sDevice;

	// ルートパラメータ
	RootParameterManager::Initialize();
	// サンプラー
	SamplerManager::Initialize();
	// インプットレイアウト
	InputLayoutManager::Initialize();
	// ブレンドステート
	BlendStateManager::Initialize();

	// パイプラインステート作る

	// 引数
	CreateDesc desc;

#pragma region モデル
	desc.pipelineStateIndex = kPipelineStateIndexModel;
	desc.rootParameterIndex = kRootParameterIndexModel;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/NormalModel.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 2;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region アニメーションモデル
	desc.pipelineStateIndex = kPipelineStateIndexAnimModel;
	desc.rootParameterIndex = kRootParameterIndexAnimModel;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 2;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 反転モデル(右手座標系)
	desc.pipelineStateIndex = kPipelineStateIndexAnimInverseModel;
	desc.rootParameterIndex = kRootParameterIndexAnimModel;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_FRONT;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 2;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region たくさんのアニメーション無しモデル
	desc.pipelineStateIndex = kPipelineStateIndexManyModels;
	desc.rootParameterIndex = kRootParameterIndexManyModels;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/ManyNormalModels.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/ManyModels.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 2;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region たくさんのアニメーションありモデル
	desc.pipelineStateIndex = kPipelineStateIndexAnimManyModels;
	desc.rootParameterIndex = kRootParameterIndexManyAnimModels;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/ManyAnimModels.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/ManyModels.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 2;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region スプライト
	desc.pipelineStateIndex = kPipelineStateIndexSprite;
	desc.rootParameterIndex = kRootParameterIndexSprite;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_NONE;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Sprite/Sprite.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Sprite/Sprite.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region パーティクル
	desc.pipelineStateIndex = kPipelineStateIndexParticle;
	desc.rootParameterIndex = kRootParameterIndexParticle;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexAdd;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Particle/Particle.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Particle/Particle.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクル
	desc.pipelineStateIndex = kPipelineStateIndexGPUParticle;
	desc.rootParameterIndex = kRootParameterIndexGPUParticle;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex = kBlendStateIndexAdd;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/GPUParticle/GPUParticle.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/GPUParticle/GPUParticle.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクルDissolveあり
	desc.pipelineStateIndex = kPipelineStateIndexGPUParticleDissolve;
	desc.rootParameterIndex = kRootParameterIndexGPUParticleDissolve;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex = kBlendStateIndexAdd;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/GPUParticle/GPUParticleDissolve.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/GPUParticle/GPUParticleDissolve.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクルブレンドノーマル
	desc.pipelineStateIndex = kPipelineStateIndexGPUParticleBlendNormal;
	desc.rootParameterIndex = kRootParameterIndexGPUParticle;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/GPUParticle/GPUParticleBlendNormal.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/GPUParticle/GPUParticle.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 2Dコライダーデバッグ
	desc.pipelineStateIndex = kPipelineStateIndexCollision2DDebugDraw;
	desc.rootParameterIndex = kRootParameterIndexCollision2DDebugDraw;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = false;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexAdd;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Collider2DDebug/Collider2DDebug.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Collider2DDebug/Collider2DDebug.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 線
	desc.pipelineStateIndex = kPipelineStateIndexLine;
	desc.rootParameterIndex = kRootParameterIndexLine;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = false;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_NONE;
	desc.fillMode = D3D12_FILL_MODE_WIREFRAME;
	desc.filePathVS = L"Resources/shaders/Line/Line.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Line/Line.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region ウィンドウスプライトSRV
	desc.pipelineStateIndex = kPipelineStateIndexWindowSpriteSRV;
	desc.rootParameterIndex = kRootParameterIndexWindowSpriteSRV;
	desc.samplerIndex = kSamplerIndexPostEffect;
	desc.depthEnable = false;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_NONE;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/WindowSprite/WindowSprite.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/WindowSprite/WindowSprite.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region スカイボックス
	desc.pipelineStateIndex = kPipelineStateIndexSkyBox;
	desc.rootParameterIndex = kRootParameterIndexSkyBox;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex = kInputLayoutIndexPositon;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Skybox/Skybox.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Skybox/Skybox.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 布
	desc.pipelineStateIndex = kPipelineStateIndexCloth;
	desc.rootParameterIndex = kRootParameterIndexCloth;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNormal;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_NONE;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Cloth/Cloth.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Cloth/Cloth.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 布GPU
	desc.pipelineStateIndex = kPipelineStateIndexClothGPU;
	desc.rootParameterIndex = kRootParameterIndexClothGPU;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_NONE;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/ClothGPU/ClothGPU.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Cloth/Cloth.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 1;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region アニメーションモデルRT2
	desc.pipelineStateIndex = kPipelineStateIndexAnimModelRT2;
	desc.rootParameterIndex = kRootParameterIndexAnimModel;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_BACK;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/ModelRT2.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 3;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 反転モデル(右手座標系)RT2
	desc.pipelineStateIndex = kPipelineStateIndexAnimInverseModelRT2;
	desc.rootParameterIndex = kRootParameterIndexAnimModel;
	desc.samplerIndex = kSamplerIndexNormal;
	desc.depthEnable = true;
	desc.depthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex = kInputLayoutIndexNone;
	desc.blendStateIndex = kBlendStateIndexNormal;
	desc.cullMode = D3D12_CULL_MODE_FRONT;
	desc.fillMode = D3D12_FILL_MODE_SOLID;
	desc.filePathVS = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS = L"Resources/shaders/Model/ModelRT2.PS.hlsl";
	desc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets = 3;
	desc.RTVFormats = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

}

void GraphicsPipelineState::Create(const CreateDesc& desc)
{

	CreatePSODesc createPSODesc;

	RootsignatureSetting(
		desc.pipelineStateIndex,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
		desc.rootParameterIndex,
		desc.samplerIndex);

	createPSODesc.pipelineStateIndex = desc.pipelineStateIndex;

	createPSODesc.depthStencilState = DepthStencilStateSetting(
		desc.depthEnable,
		desc.depthWriteMask,
		D3D12_COMPARISON_FUNC_LESS_EQUAL);

	createPSODesc.inputLayoutDesc = InputLayoutSetting(
		desc.inputLayoutIndex);

	createPSODesc.blendDesc = BlendStateSetting(desc.blendStateIndex);

	createPSODesc.rasterizerDesc = ResiterzerStateSetting(desc.cullMode, desc.fillMode);

	createPSODesc.vertexShaderBlob = CompileShader::Compile(desc.filePathVS, L"vs_6_0");
	createPSODesc.pixelShaderBlob = CompileShader::Compile(desc.filePathPS, L"ps_6_0");

	//書き込むRTVの情報
	createPSODesc.numRenderTargets = desc.numRenderTargets;
	createPSODesc.RTVFormats = desc.RTVFormats;
	//利用するトポロジ(形状)のタイプ。
	createPSODesc.primitiveTopologyType = desc.primitiveTopologyType;

	//どのように画面に色を打ち込むのかの設定
	createPSODesc.sampleDescCount = 1;
	createPSODesc.sampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilの設定
	createPSODesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	CreatePSO(createPSODesc);

}

void GraphicsPipelineState::RootsignatureSetting(PipelineStateIndex pipelineStateName, D3D12_ROOT_SIGNATURE_FLAGS rootsignatureFlags, RootParameterIndex rootParameterIndex, SamplerIndex samplerIndex)
{

	HRESULT hr;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};
	descriptionRootsignature.Flags = rootsignatureFlags;

	descriptionRootsignature.pParameters = RootParameterManager::rootParameters_[rootParameterIndex].data(); //ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = static_cast<uint32_t>(RootParameterManager::rootParameters_[rootParameterIndex].size()); //配列の長さ

	descriptionRootsignature.pStaticSamplers = SamplerManager::samplers_[samplerIndex].data();
	descriptionRootsignature.NumStaticSamplers = static_cast<uint32_t>(SamplerManager::samplers_[samplerIndex].size());

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log::Message(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に生成
	hr = sDevice_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature[pipelineStateName]));
	assert(SUCCEEDED(hr));

}

D3D12_DEPTH_STENCIL_DESC GraphicsPipelineState::DepthStencilStateSetting(bool depthEnable, D3D12_DEPTH_WRITE_MASK depthWriteMask, D3D12_COMPARISON_FUNC depthFunc)
{

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = depthEnable;
	depthStencilDesc.DepthWriteMask = depthWriteMask;
	depthStencilDesc.DepthFunc = depthFunc;
	return depthStencilDesc;

}

D3D12_INPUT_LAYOUT_DESC GraphicsPipelineState::InputLayoutSetting(InputLayoutIndex inputLayoutIndex)
{
	return InputLayoutManager::inputLayouts_[inputLayoutIndex];

}

D3D12_BLEND_DESC GraphicsPipelineState::BlendStateSetting(BlendStateIndex blendStateIndex)
{

	return BlendStateManager::blendStates_[blendStateIndex];

}

D3D12_RASTERIZER_DESC GraphicsPipelineState::ResiterzerStateSetting(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode)
{
	
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = cullMode;
	rasterizerDesc.FillMode = fillMode;
	return rasterizerDesc;

}

void GraphicsPipelineState::CreatePSO(const CreatePSODesc& createPSODesc)
{

	HRESULT hr;

	//PSOを生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = sRootSignature[createPSODesc.pipelineStateIndex].Get();//RootSignature
	graphicsPipelineStateDesc.InputLayout = createPSODesc.inputLayoutDesc;//InputLayout
	graphicsPipelineStateDesc.VS = { createPSODesc.vertexShaderBlob->GetBufferPointer(),
	createPSODesc.vertexShaderBlob->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { createPSODesc.pixelShaderBlob->GetBufferPointer(),
	createPSODesc.pixelShaderBlob->GetBufferSize() };//PixelShader
	graphicsPipelineStateDesc.BlendState = createPSODesc.blendDesc;//BlendState
	graphicsPipelineStateDesc.RasterizerState = createPSODesc.rasterizerDesc;//RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = createPSODesc.numRenderTargets;
	for (uint32_t i = 0; i < graphicsPipelineStateDesc.NumRenderTargets; ++i) {
		graphicsPipelineStateDesc.RTVFormats[i] = createPSODesc.RTVFormats;
	}

	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		createPSODesc.primitiveTopologyType;
	//どのように画面に色を打ち込むのかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = createPSODesc.sampleDescCount;
	graphicsPipelineStateDesc.SampleMask = createPSODesc.sampleMask;

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = createPSODesc.depthStencilState;
	graphicsPipelineStateDesc.DSVFormat = createPSODesc.DSVFormat;

	//実際に生成
	hr = sDevice_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&sPipelineState[createPSODesc.pipelineStateIndex]));
	assert(SUCCEEDED(hr));

}
