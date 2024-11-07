#pragma once
#include "../Collider.h"
#include <array>

class Triangle : public Collider
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<Vector3, 3>& vertices, ColliderParentObject parentObject);

	void SetVertices(const std::array<Vector3, 3>& vertices);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	std::array<Vector3, 3> vertices_;

};

