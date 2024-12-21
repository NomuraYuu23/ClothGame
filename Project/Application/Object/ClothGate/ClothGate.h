#pragma once
#include "BaseClothGate.h"

// 前方宣言
class Player;

/// <summary>
/// 布ゲート
/// </summary>
class ClothGate :
    public BaseClothGate
{

private: // 静的メンバ変数

	// 大きさ
	static const Vector2 kClothScale_;
	// 分割数
	static const Vector2 kClothDiv_;
	// ワールド座標からの左固定部分
	static const Vector3 kBaseLeftFixed_;
	// ワールド座標からの右固定部分
	static const Vector3 kBaseRightFixed_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 布リセット
	/// </summary>
	void ClothReset() override;

private: // メンバ関数

	/// <summary>
	/// 布更新
	/// </summary>
	void ClothUpdate() override;

};

