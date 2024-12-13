#pragma once
#include "Velocity2DData.h"
#include <wrl/client.h>
#include <d3d12.h>

/// <summary>
/// モーションブラーで使用する速度2Dのマネージャー
/// </summary>
class Velocity2DManager
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
	/// Velocity2Dバッファ取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetVelocity2DDataBuff() { return velocity2DDataBuff_.Get(); }

public: // アクセッサ

	/// <summary>
	/// 速度設定
	/// </summary>
	/// <param name="velocity3D">3Dでの速度</param>
	/// <param name="viewMatrix">ビュー行列</param>
	void SetVelocity(
		const Vector3& velocity3D,
		const Matrix4x4& viewMatrix);

	/// <summary>
	/// 速度設定
	/// </summary>
	/// <param name="velocity">速度</param>
	void SetVelocity(const Vector2& velocity);

private: //変数

	// デバイス
	ID3D12Device* device_ = nullptr;

	// ShockWaveバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> velocity2DDataBuff_;
	// ShockWaveマップ
	Velocity2DData* velocity2DDataMap_{};

};

