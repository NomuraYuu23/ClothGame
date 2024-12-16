#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include <cstdint>

/// <summary>
/// ライティング種類
/// </summary>
enum EnableLighting {

	None = 0, // 無し
	Lambert = 1, // ランバート
	HalfLambert = 2, // 半ランバート
	PhongReflection = 3, // フォン
	BlinnPhongReflection = 4, // ブリンフォン

};

/// <summary>
/// マテリアルデータ
/// </summary>
struct MaterialData {
	Vector4 color; // 色
	int32_t enableLighting; // ライティング種類
	float padding[3];
	Matrix4x4 uvTransform; // UVトランスフォーム
	float shininess; // 輝度
	float environmentCoefficient; // 環境係数
};

/// <summary>
/// マテリアルデータSRV版
/// </summary>
struct SRVMaterialData {
	Vector4 color; // 色
	int32_t enableLighting; // ライティング種類
	Matrix4x4 uvTransform; // UVトランスフォーム
	float shininess; // 輝度
	float environmentCoefficient; // 環境係数
};
