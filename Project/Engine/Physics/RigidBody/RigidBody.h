#pragma once
#include "../MassPoint/MassPoint.h"
#include "../../Math/Matrix/Matrix4x4.h"
#include "../../Math/Quaternion.h"
#include "../../Collider/OBB/OBB.h"

/// <summary>
/// 剛体
/// </summary>
class RigidBody
{

	/// <summary>
	/// ヤコビアン(拘束力の方向)
	/// </summary>
	struct Jacobian
	{
		Vector3 n; // 接触点での法線
		Vector3 nR; // 接触点での法線と重心から剛体上の点pに向かうベクトルのクロス積
	};

public: // 変数

	MassPoint massPoint_; // 質点

	Vector3 torque_;	// トルク(ひねり力)
	
	Matrix4x4 inertiaTensor_; // 慣性テンソル
	Matrix4x4 basicPostureInertiaTensor_; // 基本姿勢での慣性テンソル
	
	Matrix4x4 postureMatrix_; // 姿勢行列

	Vector3 angularVelocity_; // 角速度
	Vector3 angularMomentum_; // 角運動量

	Vector3 centerOfGravityVelocity_; // 重心位置速度
	Vector3 centerOfGravity_; // 重心位置 

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mass">質量</param>
	/// <param name="size">サイズ</param>
	void Initialize(float mass, const Vector3& size);

	/// <summary>
	/// 力を加える
	/// </summary>
	/// <param name="center">中心</param>
	/// <param name="pointOfAction">力を加える場所</param>
	/// <param name="force">力</param>
	void ApplyForce(const Vector3& center, const Vector3& pointOfAction, const Vector3& force);

	/// <summary>
	/// 衝突位置確認
	/// </summary>
	/// <param name="rigidBody">剛体</param>
	/// <param name="myObb">自分</param>
	/// <param name="pairObb">ペア</param>
	/// <param name="coefficientOfRestitution">反発係数</param>
	/// <param name="isGround">地面か</param>
	/// <param name="power">力</param>
	static void CollisionPositionConfirmation(RigidBody* rigidBody,
		const OBB& myObb,
		const OBB& pairObb,
		float coefficientOfRestitution,
		bool isGround,
		float power);

private:

	/// <summary>
	/// 2つの近い点を見つける
	/// </summary>
	/// <param name="ansNumber">一つ目の点</param>
	/// <param name="ansNumber2">二つ目の点</param>
	/// <param name="obbVertex">obbの頂点</param>
	/// <param name="pairPos">ペアの位置</param>
	static void FindTwoClosePoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8], const Vector3& pairPos);
	
	/// <summary>
	/// 2つの近い点を見つける
	/// </summary>
	/// <param name="ansNumber">一つ目の点</param>
	/// <param name="ansNumber2">二つ目の点</param>
	/// <param name="obbVertex">obbの頂点</param>
	static void FindTwoClosePoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8]);
	
	/// <summary>
	/// 2つの遠い点を見つける
	/// </summary>
	/// <param name="ansNumber">一つ目の点</param>
	/// <param name="ansNumber2">二つ目の点</param>
	/// <param name="obbVertex">obbの頂点</param>
	/// <param name="pairPos">ペアの位置</param>
	static void FindTwoFarPoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8], const Vector3& pairPos);
	
	/// <summary>
	/// 2つの遠い点を見つける
	/// </summary>
	/// <param name="ansNumber">一つ目の点</param>
	/// <param name="ansNumber2">二つ目の点</param>
	/// <param name="obbVertex">obbの頂点</param>
	static void FindTwoFarPoints(uint32_t& ansNumber, uint32_t& ansNumber2, const Vector3 obbVertex[8]);

public: // 関数

	/// <summary>
	/// トルク(ひねり力)計算
	/// </summary>
	/// <param name="centerOfGravity">重心</param>
	/// <param name="pointOfAction">作用点</param>
	/// <param name="force">力</param>
	/// <returns>トルク(ひねり力)</returns>
	static Vector3 TorqueCalc(
		const Vector3& centerOfGravity,
		const Vector3& pointOfAction,
		const Vector3& force);

	/// <summary>
	/// 慣性テンソル更新
	/// </summary>
	/// <param name="postureMatrix">姿勢行列</param>
	/// <param name="basicPostureInertiaTensor">基本姿勢での慣性テンソル</param>
	/// <returns>慣性テンソル</returns>
	static Matrix4x4 InertiaTensorCalc(
		const Matrix4x4& postureMatrix,
		const Matrix4x4& basicPostureInertiaTensor);

	/// <summary>
	/// 姿勢の更新
	/// </summary>
	/// <param name="postureMatrix">姿勢行列</param>
	/// <param name="angularVelocity">角速度</param>
	/// <param name="time">時間(delta)</param>
	/// <returns>姿勢行列</returns>
	static Matrix4x4 PostureCalc(
		const Matrix4x4& postureMatrix,
		const Vector3& angularVelocity,
		float time);

	/// <summary>
	/// 角運動量の更新
	/// </summary>
	/// <param name="angularMomentum">角運動量</param>
	/// <param name="torque">トルク(ひねり力)</param>
	/// <param name="time">時間(delta)</param>
	/// <returns>angularMomentum</returns>
	static Vector3 AngularMomentumCalc(
		const Vector3& angularMomentum,
		const Vector3& torque,
		float time);

	/// <summary>
	/// 角速度の更新
	/// </summary>
	/// <param name="inertiaTensor">慣性テンソル</param>
	/// <param name="angularMomentum">角運動量</param>
	/// <returns>角速度</returns>
	static Vector3 AngularVelocityCalc(
		const Matrix4x4& inertiaTensor,
		const Vector3& angularMomentum);

	/// <summary>
	/// 剛体上の点の速度を求める
	/// </summary>
	/// <param name="angularVelocity">角速度</param>
	/// <param name="centerOfGravityVelocity">重心位置の速度</param>
	/// <param name="point">剛体上の点</param>
	/// <param name="centerOfGravity">重心位置</param>
	/// <returns>剛体上の点の速度</returns>
	static Vector3 PointVelocityCalc(
		const Vector3& angularVelocity,
		const Vector3& centerOfGravityVelocity,
		const Vector3& point,
		const Vector3& centerOfGravity);

	/// <summary>
	/// ヤコビアンを求める
	/// </summary>
	/// <param name="normalize">法線</param>
	/// <param name="point">点</param>
	/// <param name="centerOfGravity">重心</param>
	/// <returns></returns>
	static Jacobian JacobianCalc(
		const Vector3& normalize,
		const Vector3& point,
		const Vector3& centerOfGravity);

public: // 確認用関数

	/// <summary>
	/// 拘束表現確認(関節)
	/// </summary>
	/// <param name="velocityA">物体Aの速度</param>
	/// <param name="velocityB">物体Bの速度</param>
	/// <returns></returns>
	bool RestraintConfirmationJoint(
		const Vector3& velocityA, 
		const Vector3& velocityB);

	/// <summary>
	/// 拘束表現確認(貫通無し)
	/// </summary>
	/// <param name="velocityA">物体Aの速度</param>
	/// <param name="velocityB">物体Bの速度</param>
	/// <param name="normalizeB">接触点での物体Bの法線</param>
	/// <returns></returns>
	bool RestraintConfirmationNoPenetration(
		const Vector3& velocityA,
		const Vector3& velocityB,
		const Vector3& normalizeB);

};

