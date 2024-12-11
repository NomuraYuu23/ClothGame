#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <list>
#include <memory>
#include "../Vertex/ColorVertexData.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include "../../Camera/BaseCamera.h"
#include "LineForGPU.h"

#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// 線描画
/// </summary>
class DrawLine
{

public: // 関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static DrawLine* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	void Initialize(
		ID3D12Device* device,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera);

	/// <summary>
	/// マップ
	/// </summary>
	/// <param name="lineForGPU">線情報</param>
	void Map(const LineForGPU& lineForGPU);

private:

	//lineForGPU用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> lineForGPUBuff_;
	//書き込むためのアドレスを取得
	LineForGPU* lineForGPUMap_{};
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE lineForGPUHandleCPU_;
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE lineForGPUHandleGPU_;
	// ディスクリプタヒープの位置
	uint32_t indexDescriptorHeap_ = 0;

	// 頂点数
	const int kVertNum_ = 2;
	// デバイス
	 ID3D12Device* device_;
	// コマンドリスト
	 ID3D12GraphicsCommandList* commandList_;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;
	// パイプラインステートオブジェクト
	ID3D12PipelineState* pipelineState_;
	
	// インスタンス数
	uint32_t numInstance_;

public: // 静的メンバ変数

	// 線最大数
	const uint32_t kNumInstanceMax_ = 4096;

private: // シングルトン

	DrawLine() = default;
	~DrawLine() = default;
	DrawLine(const DrawLine&) = delete;
	const DrawLine& operator=(const DrawLine&) = delete;

};

