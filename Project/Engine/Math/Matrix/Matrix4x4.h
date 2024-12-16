#pragma once
#include "../Vector/Vector3.h"

// 前方宣言
class Quaternion;

/// <summary>
/// 行列4x4
/// </summary>
class Matrix4x4
{

public: // メンバ変数

    float m[4][4];

public: // メンバ関数

    /// <summary>
    /// 加算
    /// </summary>
    /// <param name="m1">一個目の値</param>
    /// <param name="m2">二個目の値</param>
    /// <returns></returns>
    static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
    /// <summary>
    /// 減算
    /// </summary>
    /// <param name="m1">引かれる値</param>
    /// <param name="m2">引く値</param>
    /// <returns></returns>
    static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
    /// <summary>
    /// 積
    /// </summary>
    /// <param name="m1">一個目の値</param>
    /// <param name="m2">二個目の値</param>
    /// <returns></returns>
    static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
    /// <summary>
    /// スカラー積
    /// </summary>
    /// <param name="scalar">スカラー</param>
    /// <param name="m">行列</param>
    /// <returns></returns>
    static Matrix4x4 Multiply(float scalar, const Matrix4x4& m);
    static Matrix4x4 Multiply(const Matrix4x4& m, float scalar);
    /// <summary>
    /// 逆行列
    /// </summary>
    /// <param name="m">行列</param>
    /// <returns></returns>
    static Matrix4x4 Inverse(const Matrix4x4& m);
    /// <summary>
    /// 転置行列
    /// </summary>
    /// <param name="m">行列</param>
    /// <returns></returns>
    static Matrix4x4 Transpose(const Matrix4x4& m);
    /// <summary>
    /// 単位行列の作成
    /// </summary>
    /// <returns></returns>
    static Matrix4x4 MakeIdentity4x4();

    /// <summary>
    /// 平行移動行列
    /// </summary>
    /// <param name="translate">位置</param>
    /// <returns></returns>
    static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
    /// <summary>
    /// 拡大縮小行列
    /// </summary>
    /// <param name="scale">大きさ</param>
    /// <returns></returns>
    static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
    /// <summary>
    /// 座標変換
    /// </summary>
    /// <param name="vector">ベクトル</param>
    /// <param name="matrix">行列</param>
    /// <returns></returns>
    static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
    /// <summary>
    /// 座標変換(平行移動なし)
    /// </summary>
    /// <param name="vector">ベクトル</param>
    /// <param name="matrix">行列</param>
    /// <returns></returns>
    static Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);

    /// <summary>
    /// X軸回転行列
    /// </summary>
    /// <param name="radian">ラジアン角</param>
    /// <returns></returns>
    static Matrix4x4 MakeRotateXMatrix(float radian);
    /// <summary>
    /// Y軸回転行列
    /// </summary>
    /// <param name="radian">ラジアン角</param>
    /// <returns></returns>
    static Matrix4x4 MakeRotateYMatrix(float radian);
    /// <summary>
    /// Z軸回転行列
    /// </summary>
    /// <param name="radian">ラジアン角</param>
    /// <returns></returns>
    static Matrix4x4 MakeRotateZMatrix(float radian);
    /// <summary>
    /// 回転行列
    /// </summary>
    /// <param name="rotate">回転</param>
    /// <returns></returns>
    static Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate);

    /// <summary>
    /// 3次元アフィン変換行列
    /// </summary>
    /// <param name="scale">大きさ</param>
    /// <param name="rotate">回転</param>
    /// <param name="translate">位置</param>
    /// <returns></returns>
    static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
    static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

    /// <summary>
    /// 正射影行列
    /// </summary>
    /// <param name="left">左</param>
    /// <param name="top">上</param>
    /// <param name="right">右</param>
    /// <param name="bottom">下</param>
    /// <param name="nearClip">近く</param>
    /// <param name="farClip">遠く</param>
    /// <returns></returns>
    static Matrix4x4 MakeOrthographicMatrix(
        float left, float top, float right, float bottom, float nearClip, float farClip);

    /// <summary>
    /// 透視投影行列
    /// </summary>
    /// <param name="fovY"></param>
    /// <param name="aspectRatio"></param>
    /// <param name="nearClip"></param>
    /// <param name="farClip"></param>
    /// <returns></returns>
    static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

    /// <summary>
    /// ビューポート変換行列
    /// </summary>
    /// <param name="left">左</param>
    /// <param name="top">上</param>
    /// <param name="width">幅</param>
    /// <param name="height">高さ/param>
    /// <param name="minDepth">最小深度</param>
    /// <param name="maxDepth">最大深度</param>
    /// <returns></returns>
    static Matrix4x4 MakeViewportMatrix(
        float left, float top, float width, float height, float minDepth, float maxDepth);

    /// <summary>
    /// 任意軸回転行列の作成関数
    /// </summary>
    /// <param name="axis">軸</param>
    /// <param name="angle">回転角</param>
    /// <returns></returns>
    static Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

    /// <summary>
    /// D2D
    /// </summary>
    /// <param name="from">開始</param>
    /// <param name="to">終了</param>
    /// <returns></returns>
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
