#pragma once
#include <memory>
#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLightManager.h"
#include "../../Light/SpotLight/SpotLightManager.h"
#include "../../3D/Fog/FogManager.h"
#include "ClothPipelineState.h"
#include "ClothGPUCollision.h"

/// <summary>
/// 布マネージャー
/// </summary>
class ClothManager
{

public: // 静的メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ClothManager* GetInstance();

public: // メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sFogManager"></param>
	void Initialize(
		ID3D12Device* device,
		FogManager* sFogManager);

	/// <summary>
	/// ライト設定
	/// </summary>
	/// <param name="sDirectionalLight"></param>
	/// <param name="sPointLightManager"></param>
	/// <param name="sSpotLightManager"></param>
	void SetLight(
		DirectionalLight* sDirectionalLight,
		PointLightManager* sPointLightManager,
		SpotLightManager* sSpotLightManager);

	/// <summary>
	/// グラフィックスルート設定
	/// </summary>
	/// <param name="commandList"></param>
	void SetGraphicsRoot(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// パイプラインステート取得
	/// </summary>
	/// <returns></returns>
	ClothPipelineState* GetClothPipelineState() { return clothPipelineState_.get(); }

private: // メンバ変数

	//	平行光源
	DirectionalLight* directionalLight_;
	// ポイントライトマネージャ
	PointLightManager* pointLightManager_;
	//	スポットライトマネージャ
	SpotLightManager* spotLightManager_;
	// 霧マネージャー
	FogManager* fogManager_;

	// パイプラインステート
	std::unique_ptr<ClothPipelineState> clothPipelineState_;

	// 
	//std::unique_ptr<ClothGPUCollision> clothGPUCollision_;

private: // シングルトン
	ClothManager() = default;
	~ClothManager() = default;
	ClothManager(const ClothManager&) = delete;
	ClothManager& operator=(const ClothManager&) = delete;

};

