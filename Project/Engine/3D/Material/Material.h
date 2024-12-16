#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "../../Math/Vector/Vector2.h"
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Matrix/Matrix4x4.h"

#include "MaterialData.h"
#include "../Transform/TransformStructure.h"

/// <summary>
/// マテリアル
/// </summary>
class Material
{

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 3Dモデル生成
	/// </summary>
	/// <returns></returns>
	static Material* Create();

private:

	// デバイス
	static ID3D12Device* sDevice_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="uvTransform">UVトランスフォーム</param>
	/// <param name="color">色</param>
	/// <param name="enableLighting">ライティング種類</param>
	/// <param name="shininess">輝度</param>
	/// <param name="environmentCoefficient">環境係数</param>
	void Update(const EulerTransform& uvTransform, const Vector4& color, int enableLighting, float shininess, float environmentCoefficient);

public: //アクセッサ

	/// <summary>
	/// バッファ
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetMaterialBuff() { return materialBuff_.Get(); }

	/// <summary>
	/// マップ
	/// </summary>
	/// <returns></returns>
	MaterialData* GetMaterialMap() { return materialMap_; }
	void SetMaterialMap(MaterialData* materialMap) { materialMap_ = materialMap; }

	/// <summary>
	/// UVトランスフォーム
	/// </summary>
	/// <param name="uvTransform"></param>
	void SetUvTransform(const EulerTransform& uvTransform);
	
	/// <summary>
	/// 色
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// ライティング種類
	/// </summary>
	/// <param name="enableLighting"></param>
	void SetEnableLighting(const int32_t enableLighting);

	/// <summary>
	/// 輝度
	/// </summary>
	/// <param name="shininess"></param>
	void SetShininess(const float shininess);

	/// <summary>
	/// 環境係数
	/// </summary>
	/// <param name="environmentCoefficient"></param>
	void SetEnvironmentCoefficient(const float environmentCoefficient);

private:

	// Sprite用のマテリアルリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff_;

	// マップ
	MaterialData* materialMap_ = nullptr;

	// UVトランスフォーム
	EulerTransform uvTransform_;

	// 色
	Vector4 color_;

	// ライティング種類
	int32_t enableLighting_;

	// 輝度
	float shininess_;

	// 環境係数
	float environmentCoefficient_;

};

