#pragma once
#include "../../../Engine/GPUParticle/GPUParticle.h"

/// <summary>
/// 走る、ダッシュの時のエフェクト
/// </summary>
class RunDustParticle :
    public GPUParticle
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState) override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera) override;

private:

	/// <summary>
	/// バッファの初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void UAVBufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void InitialzieCS(ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Emit(ID3D12GraphicsCommandList* commandList) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="commandList"></param>
	void UpdateCS(ID3D12GraphicsCommandList* commandList) override;

private: // パイプラインステートの初期化CS

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void PipelineStateCSInitializeForInitialize(ID3D12Device* device) override;

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="device">デバイス</param>
	void PipelineStateCSInitializeForEmit(ID3D12Device* device) override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="device">デバイス</param>
	void PipelineStateCSInitializeForUpdate(ID3D12Device* device) override;

};

