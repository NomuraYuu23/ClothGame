#pragma once
#include "../Collider.h"
class OBB : 
	public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">中心</param>
	/// <param name="otientatuons1">座標軸X</param>
	/// <param name="otientatuons2">座標軸Y</param>
	/// <param name="otientatuons3">座標軸Z</param>
	/// <param name="size">大きさ</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector3& center,
		const Vector3& otientatuonsX, const Vector3& otientatuonsY, const Vector3& otientatuonsZ,
		const Vector3& size, ColliderParentObject parentObject);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center">中心</param>
	/// <param name="rotateMatrix">回転行列</param>
	/// <param name="size">大きさ</param>
	/// <param name="parentObject">親オブジェクト</param>
	void Initialize(const Vector3& center,
		const Matrix4x4& rotateMatrix,
		const Vector3& size, ColliderParentObject parentObject);

	/// <summary>
	/// 座標軸設定
	/// </summary>
	/// <param name="otientatuons1">座標軸X</param>
	/// <param name="otientatuons2">座標軸Y</param>
	/// <param name="otientatuons3">座標軸Z</param>
	void SetOtientatuons(const Vector3& otientatuonsX, const Vector3& otientatuonsY, const Vector3& otientatuonsZ);

	/// <summary>
	/// 座標軸設定
	/// </summary>
	/// <param name="rotateMatrix">回転行列</param>
	void SetOtientatuons(const Matrix4x4& rotateMatrix);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	// 中心
	Vector3 center_;
	// 座標軸
	Vector3 otientatuons_[3];
	// 座標軸方向の長さの半分
	Vector3 size_;

};

