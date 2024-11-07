#pragma once
#include "../Vector/Vector3.h"

class Quaternion;

class Matrix4x4
{

public: // 変数

    float m[4][4];

public: // 関数

    //加算
    static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
    //減算
    static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
    //積
    static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
    // スカラー積
    static Matrix4x4 Multiply(float scalar, const Matrix4x4& m);
    static Matrix4x4 Multiply(const Matrix4x4& m, float scalar);
    //逆行列
    static Matrix4x4 Inverse(const Matrix4x4& m);
    //転置行列
    static Matrix4x4 Transpose(const Matrix4x4& m);
    //単位行列の作成
    static Matrix4x4 MakeIdentity4x4();

    //平行移動行列
    static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
    //拡大縮小行列
    static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
    //座標変換
    static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
    //座標変換(平行移動なし)
    static Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);

    //X軸回転行列
    static Matrix4x4 MakeRotateXMatrix(float radian);
    //Y軸回転行列
    static Matrix4x4 MakeRotateYMatrix(float radian);
    //Z軸回転行列
    static Matrix4x4 MakeRotateZMatrix(float radian);
    // 回転行列
    static Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate);

    //3次元アフィン変換行列
    static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
    static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

    // 正射影行列
    static Matrix4x4 MakeOrthographicMatrix(
        float left, float top, float right, float bottom, float nearClip, float farClip);

    // 透視投影行列
    static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

    // ビューポート変換行列
    static Matrix4x4 MakeViewportMatrix(
        float left, float top, float width, float height, float minDepth, float maxDepth);

    // 任意軸回転行列の作成関数
    static Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

    // atan2みたいなもの
    static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

public: // オーバーロード

    /// <summary>
    /// +
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Matrix4x4 operator+(const Matrix4x4& v);

    /// <summary>
    /// +=
    /// </summary>
    /// <param name="v"></param>
    void operator+=(const Matrix4x4& v);

    /// <summary>
    /// -
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Matrix4x4 operator-(const Matrix4x4& v);

    /// <summary>
    /// -=
    /// </summary>
    /// <param name="v"></param>
    void operator-=(const Matrix4x4& v);

    /// <summary>
    /// *
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Matrix4x4 operator*(float v);

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
    Matrix4x4 operator*(const Matrix4x4& v);

    /// <summary>
    /// *=
    /// </summary>
    /// <param name="v"></param>
    void operator*=(const Matrix4x4& v);

};
