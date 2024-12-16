#pragma once
#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <cassert>
#include <format>
#include <wrl.h>
#include <array>
#include <vector>

#include "RootParameterManager.h"
#include "SamplerManager.h"
#include "InputLayoutManager.h"
#include "BlendStateManager.h"

/// <summary>
/// グラフィックスパイプラインステート
/// </summary>
class GraphicsPipelineState
{

public: // サブクラス

	// パイプラインの名前
	enum PipelineStateIndex {
		kPipelineStateIndexModel, // モデル
		kPipelineStateIndexAnimModel, // アニメーションモデル
		kPipelineStateIndexAnimInverseModel, // 反転モデル(右手座標系)
		kPipelineStateIndexManyModels, // たくさんのモデル
		kPipelineStateIndexAnimManyModels, // たくさんのアニメーションモデル

		kPipelineStateIndexSprite, //スプライト
		kPipelineStateIndexWindowSpriteSRV, // ウィンドウスプライトSRV
		kPipelineStateIndexLine, // 線
		kPipelineStateIndexParticle, // パーティクル
		kPipelineStateIndexGPUParticle, // GPUパーティクル
		kPipelineStateIndexGPUParticleDissolve, // GPUパーティクルDissolveあり
		kPipelineStateIndexGPUParticleBlendNormal, //GPUパーティクルブレンドノーマル

		kPipelineStateIndexCollision2DDebugDraw, // コライダーデバッグ2D
		kPipelineStateIndexSkyBox, // スカイボックス
		kPipelineStateIndexCloth, // 布
		kPipelineStateIndexClothGPU, // 布GPU


		kPipelineStateIndexAnimModelRT2, // アニメーションモデルRT2
		kPipelineStateIndexAnimInverseModelRT2, // 反転モデル(右手座標系)RT2

		kPipelineStateIndexOfCount // 使わない
	};

	// 作成用引数
	struct CreateDesc {
		PipelineStateIndex pipelineStateIndex; // パイプラインステートの名前
		RootParameterIndex rootParameterIndex; // ルートパラメータの名前
		SamplerIndex samplerIndex; // サンプラーの名前
		bool depthEnable; // 深度値をとるか
		D3D12_DEPTH_WRITE_MASK  depthWriteMask; // 深度値マスク
		InputLayoutIndex inputLayoutIndex; // インプットレイアウトの名前
		BlendStateIndex blendStateIndex; // ブレンドの名前
		D3D12_CULL_MODE cullMode; // カリング情報
		D3D12_FILL_MODE fillMode; // ワイヤーフレームかどうか
		std::wstring filePathVS; // 頂点シェーダの名前
		std::wstring filePathPS; // ピクセルシェーダの名前
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType; // 描き方（線とか三角形）
		uint32_t numRenderTargets; //RTVの数
		DXGI_FORMAT RTVFormats; //RTVの情報
	};

public: // 変数

	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState_[GraphicsPipelineState::PipelineStateIndex::kPipelineStateIndexOfCount];

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	static void Initialize(ID3D12Device* sDevice);

private: // 以下パイプラインの変数やenum

	// デバイス
	static ID3D12Device* sDevice_;

	/// <summary>
	/// PSO用
	/// </summary>
	struct CreatePSODesc {
		PipelineStateIndex pipelineStateIndex; // パイプラインステートの名前
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{}; //InputLayout
		IDxcBlob* vertexShaderBlob = nullptr; //VertexShader
		IDxcBlob* pixelShaderBlob = nullptr; //PixelShader
		D3D12_BLEND_DESC blendDesc{};//BlendStateの設定
		D3D12_RASTERIZER_DESC rasterizerDesc{};	//ResiterzerStateの設定
		uint32_t numRenderTargets; //RTVの情報
		DXGI_FORMAT RTVFormats; //RTVの情報
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType; //利用するトポロジ(形状)のタイプ。
		uint32_t sampleDescCount; //どのように画面に色を打ち込むのかの設定
		uint32_t sampleMask; //どのように画面に色を打ち込むのかの設定
		D3D12_DEPTH_STENCIL_DESC depthStencilState{}; //DepthStencilの設定
		DXGI_FORMAT DSVFormat; //DepthStencilの設定
	};

private: // パイプラインステートオブジェクト作成

	/// <summary>
	/// 作成
	/// </summary>
	static void Create(const CreateDesc& desc);

	/// <summary>
	/// RootSignature設定
	/// </summary>
	/// <param name="pipelineStateName">パイプラインステートの名前</param>
	/// <param name="rootsignatureFlags">シグネチャフラッグ</param>
	/// <param name="rootParameterIndex">ルートパラメータの名前</param>
	/// <param name="samplerIndex">サンプラーの名前</param>
	static void RootsignatureSetting(PipelineStateIndex pipelineStateName, D3D12_ROOT_SIGNATURE_FLAGS rootsignatureFlags,
		RootParameterIndex rootParameterIndex, SamplerIndex samplerIndex);

	/// <summary>
	/// DepthStencilStateの設定
	/// </summary>
	/// <param name="depthEnable">Depthの機能を有効化</param>
	/// <param name="depthWriteMask">書き込み</param>
	/// <param name="depthFunc">比較関数</param>
	/// <returns></returns>
	static D3D12_DEPTH_STENCIL_DESC DepthStencilStateSetting(bool depthEnable, D3D12_DEPTH_WRITE_MASK depthWriteMask, D3D12_COMPARISON_FUNC depthFunc);

	/// <summary>
	/// InputLayoutの設定
	/// </summary>
	/// <param name="inputLayoutIndex">インプットレイアウトの名前</param>
	/// <returns></returns>
	static D3D12_INPUT_LAYOUT_DESC InputLayoutSetting(InputLayoutIndex inputLayoutIndex);

	/// <summary>
	/// BlendStateの設定
	/// </summary>
	/// <param name="blendStateIndex">ブレンドステートの名前</param>
	/// <returns></returns>
	static D3D12_BLEND_DESC BlendStateSetting(BlendStateIndex blendStateIndex);

	/// <summary>
	/// ResiterzerStateの設定
	/// </summary>
	/// <param name="cullMode">カリング設定</param>
	/// <param name="fillMode">塗りつぶしか</param>
	/// <returns></returns>
	static D3D12_RASTERIZER_DESC ResiterzerStateSetting(D3D12_CULL_MODE cullMode, D3D12_FILL_MODE fillMode);

	/// <summary>
	/// PSOを生成 
	/// </summary>
	/// <param name="createPSODesc_">作るための引数</param>
	static void CreatePSO(const CreatePSODesc& createPSODesc_);

private: // シングルトン

	GraphicsPipelineState() = default;
	~GraphicsPipelineState() = default;
	GraphicsPipelineState(const GraphicsPipelineState&) = delete;
	GraphicsPipelineState& operator=(const GraphicsPipelineState&) = delete;

};
