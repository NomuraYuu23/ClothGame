#pragma once
#include "Vector/Vector3.h"
#include "Matrix/Matrix4x4.h"

/// <summary>
/// クォーターニオン
/// </summary>
class Quaternion
{

public:

	float x;
	float y;
	float z;
	float w;

public:

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="q0"></param>
	/// <param name="q1"></param>
	/// <returns></returns>
	static Quaternion Add(const Quaternion& q0, const Quaternion& q1);

    /// <summary>
    /// 減算
    /// </summary>
    /// <param name="q0"></param>
    /// <param name="q1"></param>
    /// <returns></returns>
    static Quaternion Subtract(const Quaternion& q0, const Quaternion& q1);

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
    
    /// <summary>
	/// 乗算
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	static Quaternion Multiply(const Quaternion& q, float s);
	static Quaternion Multiply(float s, const Quaternion& q);

	/// <summary>
	/// 単位行列の作成
	/// </summary>
	/// <returns></returns>
	static Quaternion IdentityQuaternion();

	/// <summary>
	/// 共役
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Conjugate(const Quaternion& quaternion);

	/// <summary>
	/// 正規化のための処理
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static float Norm(const Quaternion& quaternion);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Normalize(const Quaternion& quaternion);

	/// <summary>
	/// 逆クォーターニオン
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Quaternion Inverse(const Quaternion& quaternion);

	/// <summary>
	/// 軸と回転角からクォーターニオン
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	/// <summary>
	/// ベクトルを回転
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	/// <summary>
	/// 補間
	/// </summary>
	/// <param name="q0"></param>
	/// <param name="q1"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

    /// <summary>
    /// D2D
    /// </summary>
    /// <param name="v0"></param>
    /// <param name="v1"></param>
    /// <returns></returns>
    static Quaternion DirectionToDirection(const Vector3& v0, const Vector3& v1);

public: // オーバーロード

    /// <summary>
    /// +
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator+(const Quaternion& v);

    /// <summary>
    /// +=
    /// </summary>
    /// <param name="v"></param>
    void operator+=(const Quaternion& v);

    /// <summary>
    /// -
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator-(const Quaternion& v);

    /// <summary>
    /// -=
    /// </summary>
    /// <param name="v"></param>
    void operator-=(const Quaternion& v);

    /// <summary>
    /// *
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator*(float v);

    /// <summary>
    /// *=
    /// </summary>
    /// <param name="v"></param>
    void operator*=(float v);

    /// <summary>
    /// *
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Quaternion operator*(const Quaternion& v);

    /// <summary>
    /// *=
    /// </summary>
    /// <param name="v"></param>
    void operator*=(const Quaternion& v);

};
