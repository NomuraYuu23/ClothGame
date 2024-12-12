#include "GraphicsPipelineState.h"
#include "../CompileShader.h"
#include "../DxCommon/Log.h"

using namespace Microsoft::WRL;

// ルートシグネチャ
Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];
// パイプラインステートオブジェクト
Microsoft::WRL::ComPtr<ID3D12PipelineState> GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];
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
	desc.pipelineStateIndex_ = kPipelineStateIndexModel;
	desc.rootParameterIndex_ = kRootParameterIndexModel;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/NormalModel.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 2;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region アニメーションモデル
	desc.pipelineStateIndex_ = kPipelineStateIndexAnimModel;
	desc.rootParameterIndex_ = kRootParameterIndexAnimModel;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 2;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 反転モデル(右手座標系)
	desc.pipelineStateIndex_ = kPipelineStateIndexAnimInverseModel;
	desc.rootParameterIndex_ = kRootParameterIndexAnimModel;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_FRONT;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/Model.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 2;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region たくさんのアニメーション無しモデル
	desc.pipelineStateIndex_ = kPipelineStateIndexManyModels;
	desc.rootParameterIndex_ = kRootParameterIndexManyModels;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/ManyNormalModels.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/ManyModels.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 2;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region たくさんのアニメーションありモデル
	desc.pipelineStateIndex_ = kPipelineStateIndexAnimManyModels;
	desc.rootParameterIndex_ = kRootParameterIndexManyAnimModels;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/ManyAnimModels.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/ManyModels.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 2;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region スプライト
	desc.pipelineStateIndex_ = kPipelineStateIndexSprite;
	desc.rootParameterIndex_ = kRootParameterIndexSprite;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_NONE;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Sprite/Sprite.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Sprite/Sprite.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region パーティクル
	desc.pipelineStateIndex_ = kPipelineStateIndexParticle;
	desc.rootParameterIndex_ = kRootParameterIndexParticle;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexAdd;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Particle/Particle.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Particle/Particle.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクル
	desc.pipelineStateIndex_ = kPipelineStateIndexGPUParticle;
	desc.rootParameterIndex_ = kRootParameterIndexGPUParticle;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex_ = kBlendStateIndexAdd;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/GPUParticle/GPUParticle.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/GPUParticle/GPUParticle.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクルDissolveあり
	desc.pipelineStateIndex_ = kPipelineStateIndexGPUParticleDissolve;
	desc.rootParameterIndex_ = kRootParameterIndexGPUParticleDissolve;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex_ = kBlendStateIndexAdd;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/GPUParticle/GPUParticleDissolve.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/GPUParticle/GPUParticleDissolve.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region GPUパーティクルブレンドノーマル
	desc.pipelineStateIndex_ = kPipelineStateIndexGPUParticleBlendNormal;
	desc.rootParameterIndex_ = kRootParameterIndexGPUParticle;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexGPUParticle;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/GPUParticle/GPUParticleBlendNormal.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/GPUParticle/GPUParticle.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 2Dコライダーデバッグ
	desc.pipelineStateIndex_ = kPipelineStateIndexCollision2DDebugDraw;
	desc.rootParameterIndex_ = kRootParameterIndexCollision2DDebugDraw;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = false;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexAdd;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Collider2DDebug/Collider2DDebug.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Collider2DDebug/Collider2DDebug.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 線
	desc.pipelineStateIndex_ = kPipelineStateIndexLine;
	desc.rootParameterIndex_ = kRootParameterIndexLine;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = false;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_NONE;
	desc.fillMode_ = D3D12_FILL_MODE_WIREFRAME;
	desc.filePathVS_ = L"Resources/shaders/Line/Line.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Line/Line.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region ウィンドウスプライトSRV
	desc.pipelineStateIndex_ = kPipelineStateIndexWindowSpriteSRV;
	desc.rootParameterIndex_ = kRootParameterIndexWindowSpriteSRV;
	desc.samplerIndex_ = kSamplerIndexPostEffect;
	desc.depthEnable_ = false;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_NONE;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/WindowSprite/WindowSprite.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/WindowSprite/WindowSprite.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region スカイボックス
	desc.pipelineStateIndex_ = kPipelineStateIndexSkyBox;
	desc.rootParameterIndex_ = kRootParameterIndexSkyBox;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.inputLayoutIndex_ = kInputLayoutIndexPositon;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Skybox/Skybox.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Skybox/Skybox.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 布
	desc.pipelineStateIndex_ = kPipelineStateIndexCloth;
	desc.rootParameterIndex_ = kRootParameterIndexCloth;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNormal;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_NONE;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Cloth/Cloth.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Cloth/Cloth.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 布GPU
	desc.pipelineStateIndex_ = kPipelineStateIndexClothGPU;
	desc.rootParameterIndex_ = kRootParameterIndexClothGPU;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_NONE;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/ClothGPU/ClothGPU.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Cloth/Cloth.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 1;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region アニメーションモデルRT2
	desc.pipelineStateIndex_ = kPipelineStateIndexAnimModelRT2;
	desc.rootParameterIndex_ = kRootParameterIndexAnimModel;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_BACK;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/ModelRT2.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 3;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

#pragma region 反転モデル(右手座標系)RT2
	desc.pipelineStateIndex_ = kPipelineStateIndexAnimInverseModelRT2;
	desc.rootParameterIndex_ = kRootParameterIndexAnimModel;
	desc.samplerIndex_ = kSamplerIndexNormal;
	desc.depthEnable_ = true;
	desc.depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.inputLayoutIndex_ = kInputLayoutIndexNone;
	desc.blendStateIndex_ = kBlendStateIndexNormal;
	desc.cullMode_ = D3D12_CULL_MODE_FRONT;
	desc.fillMode_ = D3D12_FILL_MODE_SOLID;
	desc.filePathVS_ = L"Resources/shaders/Model/AnimModel.VS.hlsl";
	desc.filePathPS_ = L"Resources/shaders/Model/ModelRT2.PS.hlsl";
	desc.primitiveTopologyType_ = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.numRenderTargets_ = 3;
	desc.RTVFormats_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Create(desc);
#pragma endregion

}

void GraphicsPipelineState::Create(const CreateDesc& desc)
{

	CreatePSODesc createPSODesc;

	RootsignatureSetting(
		desc.pipelineStateIndex_,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
		desc.rootParameterIndex_,
		desc.samplerIndex_);

	createPSODesc.pipelineStateIndex_ = desc.pipelineStateIndex_;

	createPSODesc.depthStencilState_ = DepthStencilStateSetting(
		desc.depthEnable_,
		desc.depthWriteMask_,
		D3D12_COMPARISON_FUNC_LESS_EQUAL);

	createPSODesc.inputLayoutDesc_ = InputLayoutSetting(
		desc.inputLayoutIndex_);

	createPSODesc.blendDesc_ = BlendStateSetting(desc.blendStateIndex_);

	createPSODesc.rasterizerDesc_ = ResiterzerStateSetting(desc.cullMode_, desc.fillMode_);

	createPSODesc.vertexShaderBlob_ = CompileShader::Compile(desc.filePathVS_, L"vs_6_0");
	createPSODesc.pixelShaderBlob_ = CompileShader::Compile(desc.filePathPS_, L"ps_6_0");

	//書き込むRTVの情報
	createPSODesc.numRenderTargets_ = desc.numRenderTargets_;
	createPSODesc.RTVFormats_ = desc.RTVFormats_;
	//利用するトポロジ(形状)のタイプ。
	createPSODesc.primitiveTopologyType_ = desc.primitiveTopologyType_;

	//どのように画面に色を打ち込むのかの設定
	createPSODesc.sampleDescCount_ = 1;
	createPSODesc.sampleMask_ = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilの設定
	createPSODesc.DSVFormat_ = DXGI_FORMAT_D24_UNORM_S8_UINT;

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
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_[pipelineStateName]));
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
	graphicsPipelineStateDesc.pRootSignature = sRootSignature_[createPSODesc.pipelineStateIndex_].Get();//RootSignature
	graphicsPipelineStateDesc.InputLayout = createPSODesc.inputLayoutDesc_;//InputLayout
	graphicsPipelineStateDesc.VS = { createPSODesc.vertexShaderBlob_->GetBufferPointer(),
	createPSODesc.vertexShaderBlob_->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { createPSODesc.pixelShaderBlob_->GetBufferPointer(),
	createPSODesc.pixelShaderBlob_->GetBufferSize() };//PixelShader
	graphicsPipelineStateDesc.BlendState = createPSODesc.blendDesc_;//BlendState
	graphicsPipelineStateDesc.RasterizerState = createPSODesc.rasterizerDesc_;//RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = createPSODesc.numRenderTargets_;
	for (uint32_t i = 0; i < graphicsPipelineStateDesc.NumRenderTargets; ++i) {
		graphicsPipelineStateDesc.RTVFormats[i] = createPSODesc.RTVFormats_;
	}

	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		createPSODesc.primitiveTopologyType_;
	//どのように画面に色を打ち込むのかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = createPSODesc.sampleDescCount_;
	graphicsPipelineStateDesc.SampleMask = createPSODesc.sampleMask_;

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = createPSODesc.depthStencilState_;
	graphicsPipelineStateDesc.DSVFormat = createPSODesc.DSVFormat_;

	//実際に生成
	hr = sDevice_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&sPipelineState_[createPSODesc.pipelineStateIndex_]));
	assert(SUCCEEDED(hr));

}
