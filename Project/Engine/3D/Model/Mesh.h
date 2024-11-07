#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "../Vertex/VertexData.h"
#include "../Vertex/VertexInfluence.h"
#include "../../Animation/SkinningInformation.h"

class Mesh
{

public: // 関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Mesh();

	/// <summary>
	/// メッシュデータ生成
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	/// <param name="vertices">頂点データ</param>
	/// <param name="vertexInfluences">インフルエンスデータ</param>
	void CreateMesh(
		ID3D12Device* sDevice,
		const std::vector<VertexData>& vertices,
		const std::vector<VertexInfluence>& vertexInfluences,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 頂点バッファ
	/// </summary>
	/// <returns></returns>
	D3D12_VERTEX_BUFFER_VIEW* GetVbView() { return &vbView_; }
	void SetComputeRootDescriptorTableVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex);

	/// <summary>
	/// インフルエンスバッファ
	/// </summary>
	/// <returns></returns>
	void SetComputeRootDescriptorTableInfluenceHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex);

	/// <summary>
	/// アニメーション頂点バッファ
	/// </summary>
	/// <returns></returns>
	void SetComputeRootDescriptorTableAnimVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex);
	void SetGraphicsRootDescriptorTableAnimVertHandleGPU(ID3D12GraphicsCommandList* commandList, uint32_t rootParameterIndex);

	/// <summary>
	/// skinningバッファ
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetSkinningInformationBuff() { return skinningInformationBuff_.Get(); }

	/// <summary>
	/// マップ
	/// </summary>
	/// <returns></returns>
	SkinningInformation* GetSkinningInformationMap() { return skinningInformationMap_; }
	
	/// <summary>
	/// ピクセルシェーダ以外用のリソースに変更
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void ChangeNonPixelShaderResource(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UnordertedAccess用のリソースに変更
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void ChangeUnordertedAccess(ID3D12GraphicsCommandList* commandList);

private: // 関数

	/// <summary>
	/// 頂点バッファの初期化
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	/// <param name="vertices">頂点データ</param>
	void VertBuffInitialize(
		ID3D12Device* sDevice,
		const std::vector<VertexData>& vertices);

	/// <summary>
	/// インフルエンスバッファの初期化
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	/// <param name="vertices">インフルエンスデータ</param>
	void VertInfluenceBuffInitialize(
		ID3D12Device* sDevice,
		const std::vector<VertexInfluence>& vertexInfluences);

	/// <summary>
	/// アニメーションの頂点バッファの初期化
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	/// <param name="vertices">頂点データ</param>
	void AnimBuffInitialize(
		ID3D12Device* sDevice,
		const std::vector<VertexData>& vertices,
		ID3D12GraphicsCommandList* commandList);

private:

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	VertexData* vertMap_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE vertHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE vertHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t vertIndexDescriptorHeap_ = 0;

	// インフルエンスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceBuff_;
	// インフルエンスバッファマップ
	VertexInfluence* influenceMap_ = nullptr;

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE influenceHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE influenceHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t influenceIndexDescriptorHeap_ = 0;

	// UAVバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> animVertBuff_;

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE animVertUAVHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE animVertUAVHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t animVertUAVIndexDescriptorHeap_ = 0;

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE animVertSRVHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE animVertSRVHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t animVertSRVIndexDescriptorHeap_ = 0;

	// SkinningInformationバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> skinningInformationBuff_;
	// SkinningInformationバッファマップ
	SkinningInformation* skinningInformationMap_ = nullptr;

public:

	static void StaticInitialize(ID3D12Device* device);

	static void PipelineStateCSInitialize(ID3D12Device* device);

public:

	// ルートシグネチャCS
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineStatesCS_;

};

