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
	static ID3D12Device* sDevice;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const EulerTransform& uvTransform, const Vector4& color, int enableLighting, float shininess, float environmentCoefficient);

public: //アクセッサ

	ID3D12Resource* GetMaterialBuff() { return materialBuff_.Get(); }

	MaterialData* GetMaterialMap() { return materialMap_; }

	void SetMaterialMap(MaterialData* materialMap) { materialMap_ = materialMap; }

public:

	void SetUvTransform(const EulerTransform& uvTransform);
	void SetColor(const Vector4& color);
	void SetEnableLighting(const int32_t enableLighting);
	void SetShininess(const float shininess);
	void SetEnvironmentCoefficient(const float environmentCoefficient);

private:

	//Sprite用のマテリアルリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff_;

	MaterialData* materialMap_ = nullptr;

	EulerTransform uvTransform_;

	Vector4 color_;

	int32_t enableLighting_;

	float shininess_;

	float environmentCoefficient_;

};

