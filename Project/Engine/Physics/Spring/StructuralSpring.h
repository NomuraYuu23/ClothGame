#pragma once
#include "../MassPoint/MassPoint.h"

/// <summary>
/// 構成バネ
/// </summary>
class StructuralSpring
{

public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="point0">質点0</param>
	/// <param name="point1">質点1</param>
	/// <param name="naturalLength">自然長</param>
	/// <param name="stiffness">剛性。バネ定数k</param>
	/// <param name="dampingCoefficient">減衰係数</param>
	void Initialize(
		const MassPoint& point0,
		const MassPoint& point1,
		float naturalLength,
		float stiffness,
		float dampingCoefficient);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="wind">風</param>
	/// <param name="gravity">重力</param>
	void Update(const Vector3& wind = Vector3{ 0.0f,0.0f,0.0f },
		const Vector3& gravity = Vector3{ 0.0f,-9.8f,0.0f });

	/// <summary>
	/// 位置制限
	/// </summary>
	void  PositionLimit();

public: // アクセッサ

	// 質点0
	void SetPoint0(const MassPoint& point0) { point0_ = point0; }
	MassPoint GetPoint0() { return point0_; }
	// 質点1
	void SetPoint1(const MassPoint& point1) { point1_ = point1; }
	MassPoint GetPoint1() { return point1_; }
	// 自然長
	void SetNaturalLength(float naturalLength) { naturalLength_ = naturalLength; }
	float GetNaturalLength() { return naturalLength_; }
	// 剛性。バネ定数k
	void SetStiffness(float stiffness) { stiffness_ = stiffness; }
	float GetStiffness() { return stiffness_; }
	// 減衰係数
	void SetDampingCoefficient(float dampingCoefficient) { dampingCoefficient_ = dampingCoefficient; }
	float GetDampingCoefficient() { return dampingCoefficient_; }
	// 質点0を固定するか
	void SetFixPoint0(bool fixPoint0) { fixPoint0_ = fixPoint0; }
	bool GetFixPoint0() { return fixPoint0_; }
	// 質点1を固定するか
	void SetFixPoint1(bool fixPoint1) { fixPoint1_ = fixPoint1; }
	bool GetFixPoint1() { return fixPoint1_; }
	// 親
	void SetParent(StructuralSpring* parent) { parent_ = parent; }
	StructuralSpring* GetParent() { return parent_; }
	// 名前
	void SetName(const std::string& name);
	std::string GetName() { return name_; }
	// Y
	uint32_t GetY() { return y_; }
	// X
	uint32_t GetX() { return x_; }
	// 軸
	std::string GetAxis() { return axis_; }
	// 種類 
	std::string GetKinds() { return kinds_; }

private: // 変数

	// 質点0
	MassPoint point0_;
	// 質点1
	MassPoint point1_;
	// 自然長
	float naturalLength_;
	// 剛性。バネ定数k
	float stiffness_;
	// 減衰係数
	float dampingCoefficient_;

	// 質点0を固定するか
	bool fixPoint0_;
	// 質点1を固定するか
	bool fixPoint1_;

	// 最大距離
	float lengthMax_;

	// 名前
	std::string name_;

	// 親
	StructuralSpring* parent_;

	// Y
	uint32_t y_ = 0;
	// X
	uint32_t x_ = 0;

	// 軸
	std::string axis_ = "";

	// 種類 
	std::string kinds_ = "";

};

