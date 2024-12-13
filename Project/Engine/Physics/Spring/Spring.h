#pragma once

#include "../../Math/Vector/Vector3.h"
#include "../../Physics/MassPoint/MassPoint.h"

/// <summary>
/// バネ
/// </summary>
class Spring {

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="anchor">アンカー</param>
	/// <param name="naturalLength">自然長</param>
	/// <param name="stiffness">剛性。バネ定数k</param>
	/// <param name="dampingCoefficient">減衰係数</param>
	/// <param name="massPoint">質点</param>
	void Initialize(
		const Vector3& anchor,
		float naturalLength,
		float stiffness,
		float dampingCoefficient,
		const MassPoint& massPoint);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ

	// アンカー。固定された端の位置
	void SetAnchor(const Vector3& anchor) { anchor_ = anchor; }
	Vector3 GetAnchor() { return anchor_; }
	// 自然長
	void SetNaturalLength(float naturalLength) { naturalLength_ = naturalLength; }
	float GetNaturalLength() { return naturalLength_; }
	// 剛性。バネ定数k
	void SetStiffness(float stiffness) { stiffness_ = stiffness; }
	float GetStiffness() { return stiffness_; }
	// 減衰係数
	void SetDampingCoefficient(float dampingCoefficient) { dampingCoefficient_ = dampingCoefficient; }
	float GetDampingCoefficient() { return dampingCoefficient_; }
	// 質点
	void GetMassPoint(const MassPoint& massPoint ) { massPoint_ = massPoint; }
	MassPoint GetMassPoint() { return massPoint_; }

private: // 変数

	// アンカー。固定された端の位置
	Vector3 anchor_{};
	// 自然長
	float naturalLength_ = 0.0f;
	// 剛性。バネ定数k
	float stiffness_ = 0.0f;
	// 減衰係数
	float dampingCoefficient_ = 0.0f;
	// 質点
	MassPoint massPoint_{};

};
