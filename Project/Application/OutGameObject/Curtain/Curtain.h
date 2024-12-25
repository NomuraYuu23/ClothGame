#pragma once
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../../Engine/base/DxCommon/DirectXCommon.h"

/// <summary>
/// カーテン
/// </summary>
class Curtain
{

public: // 静的メンバ変数

	// 大きさ
	static const Vector2 kClothScale_;
	// 分割数
	static const Vector2 kClothDiv_;
	// ワールド座標からの固定部分
	static const Vector3 kBaseFixed_;
	// ワールド座標から移動部分、開く
	static const Vector3 kBaseMovingOpen_;
	// ワールド座標から移動部分、閉じる
	static const Vector3 kBaseMovingClose_;
	// DirectX
	static DirectXCommon* dxCommon_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera);

private: // メンバ変数

	// 右カーテン
	std::unique_ptr<ClothGPU> rightCurtain_;
	// 右の移動位置
	Vector3 rightMovingPosition_;

	// 左カーテン
	std::unique_ptr<ClothGPU> leftCurtain_;
	// 左の移動位置
	Vector3 leftMovingPosition_;

};

