#pragma once
#include <cstdint>
#include <list>
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include "../Transform/TransformationMatrix.h"
#include "../../Animation/LocalMatrixManager.h"
#include "../Model/ModelNodeData.h"
#include "../Model/Model.h"
#include "OneOfManyObjects.h"

/// <summary>
/// 同じアニメーションをする、またはアニメーションをしない
/// 大量のオブジェクトを管理する基底クラス
/// </summary>
class LargeNumberOfObjects
{

public: // 定数

	// 最大数
	static const uint32_t kNumInstanceMax_ = 256;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LargeNumberOfObjects();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	virtual void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map(const Matrix4x4& viewProjectionMatrix);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="textureHnadles">テクスチャハンドル</param>
	virtual void Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles = nullptr);

	/// <summary>
	/// 死んでるのを削除
	/// </summary>
	void DeadDelete();

	/// <summary>
	/// オブジェクトの追加
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void AddObject(OneOfManyObjects* object);

	/// <summary>
	/// オブジェクトの参照
	/// </summary>
	/// <returns></returns>
	std::list<std::unique_ptr<OneOfManyObjects>>* GetObjects() { return &objects_; }

protected: // トランスフォーム関係

	// トランスフォームマトリックス
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixesBuff_;
	//書き込むためのアドレスを取得
	TransformationMatrix* transformationMatrixesMap_{};
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE transformationMatrixesHandleCPU_;
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE transformationMatrixesHandleGPU_;
	// ディスクリプタヒープの位置
	uint32_t transformationMatrixesIndexDescriptorHeap_ = 0;

	// オブジェクト一つ
	std::list<std::unique_ptr<OneOfManyObjects>> objects_;
	
	//インスタンス数
	uint32_t numInstance_ = 0;

protected: // モデル関係

	// ローカル行列マネージャー
	std::unique_ptr<LocalMatrixManager> localMatrixManager_;

	// モデル
	Model* model_ = nullptr;

	//Sprite用のマテリアルリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialsBuff_;
	//書き込むためのアドレスを取得
	SRVMaterialData* materialsMap_{};
	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE materialsHandleCPU_{};
	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE materialsHandleGPU_{};
	// ディスクリプタヒープの位置
	uint32_t materialsIndexDescriptorHeap_ = 0;
};

