#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "LocalMatrix.h"
#include "../3D/Model/ModelNodeData.h"
#include "../3D/Model/ModelNode.h"

class LocalMatrixManager
{

public:

	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LocalMatrixManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelNode">モデルのノード</param>
	void Initialize(const ModelNode& modelNode);

	/// <summary>
	/// マップ
	/// </summary>
	void Map();

	/// <summary>
	/// マップ
	/// </summary>
	void Map2();
	
	/// <summary>
	/// GPUに送る
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootParameterIndex">ルートパラメータインデックス</param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex);

	/// <summary>
	/// ノードデータ設定
	/// </summary>
	/// <param name="modelNode">モデルのノード</param>
	void SetNodeDatas(const ModelNode& modelNode, int32_t parentIndex);

	/// <summary>
	/// ノードの名前
	/// </summary>
	/// <returns></returns>
	std::vector<std::string> GetNodeNames();

	/// <summary>
	/// 初期トランスフォーム取得
	/// </summary>
	/// <returns></returns>
	std::vector<QuaternionTransform> GetInitTransform();

	/// <summary>
	/// ローカル行列設定
	/// </summary>
	/// <param name="matrix">行列</param>
	void SetNodeLocalMatrix(const std::vector<Matrix4x4> matrix);

	/// <summary>
	/// ノードデータ取得
	/// </summary>
	/// <returns></returns>
	std::vector<NodeData> GetNodeDatas() { return nodeDatas_; }

	/// <summary>
	/// ノードデータ取得アドレス
	/// </summary>
	/// <returns></returns>
	std::vector<NodeData>* GetNodeDatasAddress() { return &nodeDatas_; }

	/// <summary>
	/// 数取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetNum() { return num_; }

public:

	// local
	Microsoft::WRL::ComPtr<ID3D12Resource> localMatrixesBuff_;
	//書き込むためのアドレスを取得
	LocalMatrix* localMatrixesMap_{};
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE localMatrixesHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE localMatrixesHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t indexDescriptorHeap_ = 0;

	//数
	uint32_t num_ = 0;

	// ノードデータ
	std::vector<NodeData> nodeDatas_;

};

