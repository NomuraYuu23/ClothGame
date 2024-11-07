#pragma once
#include "../../Math/Vector/Vector3.h"

class MassPoint
{

public: // 変数

	Vector3 force; // 力
	float mass; // 質量
	Vector3 position; // 位置
	Vector3 velocity; // 速度
	Vector3 acceleration; // 加速度

public: // 関数

	
	/// <summary>
	/// 加速度計算
	/// </summary>
	/// <param name="force">力</param>
	/// <param name="mass">質量</param>
	/// <returns>加速度</returns>
	static Vector3 AccelerationCalc(
		const Vector3& force,
		float mass);

	/// <summary>
	/// 速度計算
	/// </summary>
	/// <param name="velocity">現在の速度</param>
	/// <param name="acceleration">加速度</param>
	/// <param name="time">経過時間</param>
	/// <returns>速度</returns>
	static Vector3 VelocityCalc(
		const Vector3& velocity,
		const Vector3& acceleration,
		float time);

	/// <summary>
	/// 位置計算
	/// </summary>
	/// <param name="position">現在の位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="time">経過時間</param>
	/// <returns>位置</returns>
	static Vector3 PositionCalc(
		const Vector3& position,
		const Vector3& velocity,
		float time);


};

