#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>

#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// 布パイプラインCS
/// </summary>
class ClothPipelineState
{

public: // サブクラス

	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPipelineStateCSIndexInitVertex,// 初期化 頂点
		kPipelineStateCSIndexInitMassPoint,// 初期化 質点
		kPipelineStateCSIndexInitSurface,// 初期化 面

		kPipelineStateCSIndexUpdateExternalOperation,// 更新 外部操作フェーズ
		kPipelineStateCSIndexUpdateIntegral,// 更新 積分フェーズ
		kPipelineStateCSIndexUpdateSpring,// 更新 バネフェーズ
		kPipelineStateCSIndexUpdateSurface,// 更新 面
		kPipelineStateCSIndexUpdateVertex,// 更新 頂点フェーズ

		kPipelineStateCSIndexOfCount // 数える用
	};

private: // 変数

	// ルートシグネチャCS
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kPipelineStateCSIndexOfCount> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineStateCSIndexOfCount> pipelineStatesCS_;

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// パイプラインステート設定
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="index"></param>
	void SetPipelineState(ID3D12GraphicsCommandList* commandList, PipelineStateCSIndex index);

private: // CSの初期化、設定関数

	/// <summary>
	/// 初期化 頂点
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForInitVertex(ID3D12Device* device);

	/// <summary>
	/// 初期化 質点
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForInitMassPoint(ID3D12Device* device);

	/// <summary>
	/// 初期化 面
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForInitSurface(ID3D12Device* device);

	/// <summary>
	/// 更新 外部操作フェーズ
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdateExternalOperation(ID3D12Device* device);

	/// <summary>
	/// 更新 積分フェーズ
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdateIntegral(ID3D12Device* device);

	/// <summary>
	/// 更新 バネフェーズ
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdateSpring(ID3D12Device* device);

	/// <summary>
	/// 更新 面
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdateSurface(ID3D12Device* device);

	/// <summary>
	/// 更新 頂点フェーズ
	/// </summary>
	/// <param name="device"></param>
	void PipelineStateCSInitializeForUpdateVertex(ID3D12Device* device);

	/// <summary>
	/// パイプラインステートの初期化 共通関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="rootParameters">ルートパラメータ</param>
	/// <param name="numParameters">ルートパラメータ数</param>
	/// <param name="pipelineStateCSIndex">どのパイプラインか</param>
	/// <param name="filePath">HLSLのファイルパス</param>
	void PipelineStateCSCommonInitialize(
		ID3D12Device* device,
		D3D12_ROOT_PARAMETER* rootParameters,
		uint32_t numParameters,
		PipelineStateCSIndex pipelineStateCSIndex,
		const std::wstring& filePath);


};

