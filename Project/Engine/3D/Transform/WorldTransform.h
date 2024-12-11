#pragma once
#include "TransformStructure.h"
#include "../../Math/Matrix/Matrix4x4.h"

#include "TransformationMatrix.h"
#include <wrl.h>
#include <d3d12.h>
#include "../Model/ModelNode.h"
#include "../Model/ModelNodeData.h"

/// <summary>
/// ワールドトランスフォーム
/// </summary>
class WorldTransform
{

public:

	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WorldTransform();

	/// <summary>
	/// 初期化(モデル表示で使わない)
	/// </summary>
	void Initialize();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelNode"></param>
	void Initialize(const ModelNode& modelNode);

	/// <summary>
	/// 行列更新
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 行列更新
	/// </summary>
	/// <param name="rotateMatrix">回転行列</param>
	void UpdateMatrix(const Matrix4x4& rotateMatrix);

	/// <summary>
	/// マップ
	/// </summary>
	void Map(const Matrix4x4& viewProjectionMatrix);

	/// <summary>
	/// ワールドポジション取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(WorldTransform* parent) { parent_ = parent; }

	/// <summary>
	/// トランスフォームバッファ取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetTransformationMatrixBuff() { return transformationMatrixBuff_.Get(); }

public:

	//トランスフォーム
	EulerTransform transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	//ワールド行列
	Matrix4x4 worldMatrix_{};

	// 回転行列
	Matrix4x4 rotateMatrix_{};

	// 方向ベクトルで回転行列
	bool usedDirection_ = false;

	// 方向ベクトル
	Vector3 direction_ = {0.0f,0.0f,1.0f};

	// スケールを考えない
	Matrix4x4 parentMatrix_{};

	//親
	WorldTransform* parent_ = nullptr;

	//WVP用のリソースを作る。
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff_;
	
	//書き込むためのアドレスを取得
	TransformationMatrix* transformationMatrixMap_{};

};
