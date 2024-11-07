#pragma once
#include "FogData.h"
#include <wrl/client.h>
#include <d3d12.h>

class FogManager
{

public: // 関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static FogManager* GetInstance();

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
	/// fogバッファ取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetFogDataBuff() { return fogDataBuff_.Get(); }

public: // アクセッサ

	/// <summary>
	/// 色設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { fogDataMap_->color = color; }

	/// <summary>
	/// 開始位置設定
	/// </summary>
	/// <param name="fogNear">歪み</param>
	void SetNear(float fogNear) { fogDataMap_->fagNear = fogNear; }

	/// <summary>
	/// 終了位置設定
	/// </summary>
	/// <param name="fagFar">終了位置</param>
	void SetRadius(float fagFar) { fogDataMap_->fagFar = fagFar; }

private: //変数

	// デバイス
	ID3D12Device* device_;

	// ShockWaveバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> fogDataBuff_;
	// ShockWaveマップ
	FogData* fogDataMap_{};

private: // シングルトン
	FogManager() = default;
	~FogManager() = default;
	FogManager(const FogManager&) = delete;
	const FogManager& operator=(const FogManager&) = delete;

};

