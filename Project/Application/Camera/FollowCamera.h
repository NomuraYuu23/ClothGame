#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/Camera/BaseCamera.h"

// 前方宣言
class LockOn;

class FollowCamera : public BaseCamera{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(float elapsedTime = 0.0f) override;

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

public: // アクセッサ

	/// <summary>
	/// 目指すアングル
	/// </summary>
	/// <param name="destinationAngle">目指すアングル</param>
	void SetDestinationAngle(const Vector3& destinationAngle) { destinationAngle_ = destinationAngle; }

private: // メンバ関数

	/// <summary>
	/// 追従対象からのオフセットを計算する
	/// </summary>
	/// <returns></returns>
	Vector3 OffsetCalc();

	/// <summary>
	/// ロックオンではない時の更新
	/// </summary>
	void NotLockOnUpdate();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

private:

	//追従対象
	const WorldTransform* target_ = nullptr;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};

	// 目指すアングル
	Vector3 destinationAngle_ = { 0.0f,0.0f,0.0f };

	// 移動レート
	float moveRate_ = 0.2f;
	
	// 回転レート
	float rotateRate_ = 0.1f;

	// オフセットの長さ
	float offsetLength_ = -10.0f;

};

