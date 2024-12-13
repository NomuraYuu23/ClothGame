#pragma once
#include <vector>
#include "../../../externals/nlohmann/json.hpp"

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3
{

public: // メンバ変数

	float x;
	float y;
	float z;

public: // メンバ関数

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <returns></returns>
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	
	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="v1">引かれる値</param>
	/// <param name="v2">引く値</param>
	/// <returns></returns>
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	
	/// <summary>
	/// スカラー倍
	/// </summary>
	/// <param name="v">掛けられる値</param>
	/// <param name="scalar">掛ける値</param>
	/// <returns></returns>
	static Vector3 Multiply(const Vector3& v, float scalar);
	static Vector3 Multiply(float scalar, const Vector3& v);
	
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <returns></returns>
	static float Dot(const Vector3& v1, const Vector3& v2);
	
	/// <summary>
	/// 長さ（ノルム）
	/// </summary>
	/// <param name="v">値</param>
	/// <returns></returns>
	static float Length(const Vector3& v);
	
	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v">値</param>
	/// <returns></returns>
	static Vector3 Normalize(const Vector3& v);
	
	/// <summary>
	/// 最大値正規化
	/// </summary>
	/// <param name="v">値</param>
	/// <returns></returns>
	static Vector3 MaximumNormalize(const Vector3& v);
	
	/// <summary>
	/// クロス積
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <returns></returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <param name="t">補間係数</param>
	/// <returns></returns>
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	/// <summary>
	/// 3次スプライン曲線
	/// </summary>
	/// <param name="controlPoints">制御点</param>
	/// <param name="t">補間係数</param>
	/// <returns></returns>
	static Vector3 CatmullRomSpline(
		const std::vector<Vector3>& controlPoints, const float& t);

	/// <summary>
	/// 反射ベクトル
	/// </summary>
	/// <param name="input">入力</param>
	/// <param name="normal">法線</param>
	/// <returns></returns>
	static Vector3 Reflect(const Vector3& input, const Vector3& normal);

	/// <summary>
	/// 値が一番でかいのを取ってくる
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	static float GetAbsMax(const Vector3& v);

public: // オーバーロード

	/// <summary>
	/// +
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 operator+(const Vector3& v);

	/// <summary>
	/// +=
	/// </summary>
	/// <param name="v"></param>
	void operator+=(const Vector3& v);

	/// <summary>
	/// -
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 operator-(const Vector3& v);

	/// <summary>
	/// -=
	/// </summary>
	/// <param name="v"></param>
	void operator-=(const Vector3& v);

	/// <summary>
	/// *
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector3 operator*(float v);

	/// <summary>
	/// *=
	/// </summary>
	/// <param name="v"></param>
	void operator*=(float v);

};

inline void to_json(nlohmann::json& json, const Vector3& value) {
	json = nlohmann::json::array({ value.x, value.y, value.z });
}

inline void from_json(const nlohmann::json& json, Vector3& value) {
	if (json.is_array() && json.size() == 3) {
		// float型のjson配列登録
		value = { json.at(0), json.at(1), json.at(2) };
	}
}
