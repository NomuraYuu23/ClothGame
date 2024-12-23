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
		ID3D12PipelineState* pipelineState,
		const std::string name = "Particle") override;


};
