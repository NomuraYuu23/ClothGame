#pragma once
#include "../Collider.h"
#include <array>

/// <summary>
/// 衝突オブジェクト 三角面
/// </summary>
class Triangle : 
	public Collider
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="vertices">頂点</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const std::array<Vector3, 3>& vertices, ColliderParentObject parentObject);

	/// <summary>
	/// 頂点設定
	/// </summary>
	/// <param name="vertices">頂点</param>
	void SetVertices(const std::array<Vector3, 3>& vertices);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	// 頂点
	std::array<Vector3, 3> vertices_;

};

