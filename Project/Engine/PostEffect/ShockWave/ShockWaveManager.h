#pragma once
#include "ShockWaveData.h"
#include <wrl/client.h>
#include <d3d12.h>

class ShockWaveManager
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	
	/// <summary>
	/// ShockWaveバッファ取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetShockWaveDataBuff() { return shockWaveDataBuff_.Get(); }

public: // アクセッサ

	/// <summary>
	/// 中心設定
	/// </summary>
	/// <param name="center">中心</param>
	void SetCenter(const Vector2& center) { shockWaveDataMap_->center = center; }

	/// <summary>
	/// 歪み設定
	/// </summary>
	/// <param name="distortion">歪み</param>
	void SetDistortion(float distortion) { shockWaveDataMap_->distortion = distortion; }

	/// <summary>
	/// 半径設定
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { shockWaveDataMap_->radius = radius; }

	/// <summary>
	/// 厚み設定
	/// </summary>
	/// <param name="thickness">厚み</param>
	void SetThickness(float thickness) { shockWaveDataMap_->thickness = thickness; }

	/// <summary>
	/// 半径最大設定
	/// </summary>
	/// <param name="radiusMax">半径最大</param>
	void SetRadiusMax(float radiusMax) { radiusMax_ = radiusMax; }

	/// <summary>
	/// 半径の加算値設定
	/// </summary>
	/// <param name="radiusAddValue">半径の加算値</param>
	void SetRadiusAddValue(float radiusAddValue) { radiusAddValue_ = radiusAddValue; }

private: //変数

	// デバイス
	ID3D12Device* device_ = nullptr;

	// ShockWaveバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> shockWaveDataBuff_;
	// ShockWaveマップ
	ShockWaveData* shockWaveDataMap_{};

	float radiusMax_ = 0.0f; // 半径最大
	float radiusAddValue_ = 0.0f; // 半径の加算値

};

