#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"

class FollowCamera :
	public BaseCamera
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float elapsedTime = 0.0f) override;

public: // アクセッサ

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const WorldTransform* target);

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetRotateMatrix();

private: // メンバ関数

	/// <summary>
	/// 追従対象からのオフセットを計算する
	/// </summary>
	/// <returns></returns>
	Vector3 OffsetCalc();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private: // メンバ変数

	//追従対象
	const WorldTransform* target_ = nullptr;

	// オフセットの長さ
	float offsetLength_ = -10.0f;
	
	// オフセットの高さ
	float offsetHeight_ = 3.0f;

	// ターゲット位置
	Vector3 interTarget_;

};

