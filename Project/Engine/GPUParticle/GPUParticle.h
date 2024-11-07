#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include "../3D/Model/Model.h"
#include "GPUPerticleView.h"
#include "EmitterCS.h"
#include "../Math/PerFrame.h"

#pragma comment(lib, "dxcompiler.lib")

class GPUParticle
{

public:

	/// <summary>
	/// CSのパイプラインステート
	/// </summary>
	enum PipelineStateCSIndex {
		kPipelineStateCSIndexInitialize, // 初期化
		kPipelineStateCSIndexEmit, // エミット
		kPipelineStateCSIndexUpdate, // 更新
		kPipelineStateCSIndexOfCount // 数える用
	};


public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialzie();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootSignature">ルートシグネチャ</param>
	/// <param name="pipelineState">パイプラインステート</param>
	virtual void Initialize(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		ID3D12RootSignature* rootSignature,
		ID3D12PipelineState* pipelineState);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	virtual void Draw(
		ID3D12GraphicsCommandList* commandList,
		BaseCamera& camera);

	/// <summary>
	/// エミッタ情報設定
	/// </summary>
	/// <param name="emitter">エミッター</param>
	/// <param name="isEmitSet">エミッター情報をセットするか</param>
	virtual void SetEmitter(const EmitterCS& emitter, bool isEmitSet = true);

protected:

	/// <summary>
	/// パイプラインステートの初期化CS
	/// </summary>
	void PipelineStateCSInitialize(ID3D12Device* device);

	/// <summary>
	/// バッファの初期化
	/// </summary>
	/// <param name="device"></param>
	virtual void UAVBufferInitialize(ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// モデルの初期化
	/// </summary>
	void MaterialInitialize();

	/// <summary>
	/// 定数バッファ初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	virtual void ConstantBufferInitialzie(ID3D12Device* device);

	/// <summary>
	/// GPUParticleViewマッピング
	/// </summary>
	/// <param name="camera">カメラ</param>
	void GPUParticleViewMapping(BaseCamera& camera);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	virtual void InitialzieCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	virtual void Emit(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="commandList"></param>
	virtual void UpdateCS(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVバリア―
	/// </summary>
	/// <param name="commandList"></param>
	void UAVBarrier(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// リソースバリア
	/// </summary>
	/// <param name="commandList"></param>
	virtual void ResouseBarrierToNonPixelShader(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// リソースバリア
	/// </summary>
	/// <param name="commandList"></param>
	virtual void ResouseBarrierToUnorderedAccess(ID3D12GraphicsCommandList* commandList);

protected: // パイプラインステートの初期化CS

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	virtual void PipelineStateCSInitializeForInitialize(ID3D12Device* device);

	/// <summary>
	/// エミット
	/// </summary>
	/// <param name="device"></param>
	virtual void PipelineStateCSInitializeForEmit(ID3D12Device* device);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="device"></param>
	virtual void PipelineStateCSInitializeForUpdate(ID3D12Device* device);

protected:

	// パーティクルの最大数
	static const uint32_t kParticleMax;
	// モデルのディレクトリパス
	static const std::string kModelDirectoryPath;
	// テクスチャのディレクトリパス
	static const std::string kTextureDirectoryPath;
	// モデルのファイルの名前
	static const std::string kFilename;
	// モデル
	static std::unique_ptr<Model> model_;

	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buff_;
	//UAV
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE uavHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE uavHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t uavIndexDescriptorHeap_ = 0;
	// SRV
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t srvIndexDescriptorHeap_ = 0;

	// ルートシグネチャCS
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kPipelineStateCSIndexOfCount> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineStateCSIndexOfCount> pipelineStatesCS_;

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature_;
	// パイプラインステートオブジェクト
	ID3D12PipelineState* pipelineState_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// テクスチャ
	uint32_t textureHandle_;

	// テクスチャ名前
	std::string textureFilename_ = "";

	// GPUParticleViewバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> gpuParticleViewBuff_;
	// GPUParticleViewマップ
	GPUParticleView* gpuParticleViewMap_ = nullptr;

	// エミッタバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterBuff_;
	// エミッタマップ
	EmitterCS* emitterMap_ = nullptr;

	// 時間バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameBuff_;
	// 時間マップ
	PerFrame* perFrameMap_ = nullptr;

	// フリーリストインデックスUAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListIndexBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE freeListIndexHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE freeListIndexHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t freeListIndexDescriptorHeap_ = 0;

	// フリーリストUAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListBuff_;
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE freeListHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE freeListHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t freeListDescriptorHeap_ = 0;

public:

	Material* GetMaterial() { return material_.get(); }

};

