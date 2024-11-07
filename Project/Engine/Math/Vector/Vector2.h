#pragma once
#include "../../../externals/nlohmann/json.hpp"

class Vector2
{

public: // 変数

	float x;
	float y;

public: // 関数

	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <returns></returns>
	static Vector2 Add(const Vector2& v1, const Vector2& v2);
	
	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="v1">引かれる値</param>
	/// <param name="v2">引く値</param>
	/// <returns></returns>
	static Vector2 Subtract(const Vector2& v1, const Vector2& v2);
	
	/// <summary>
	/// スカラー倍
	/// </summary>
	/// <param name="v">掛けられる値</param>
	/// <param name="scalar">掛ける値</param>
	/// <returns></returns>
	static Vector2 Multiply(const Vector2& v, float scalar);
	
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1">一個目の値</param>
	/// <param name="v2">二個目の値</param>
	/// <returns></returns>
	static float Dot(const Vector2& v1, const Vector2& v2);
	
	/// <summary>
	/// 長さ
	/// </summary>
	/// <param name="v">値</param>
	/// <returns></returns>
	static float Length(const Vector2& v);
	
	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v">値</param>
	/// <returns></returns>
	static Vector2 Normalize(const Vector2& v);
	
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1">見ている値</param>
	/// <param name="v2">見られている値</param>
	/// <returns>マイナス：右 プラス：左</returns>
	static float Cross(const Vector2& v1, const Vector2& v2);

public: // オーバーロード

	/// <summary>
	/// +
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector2 operator+(const Vector2& v);

	/// <summary>
	/// +=
	/// </summary>
	/// <param name="v"></param>
	void operator+=(const Vector2& v);

	/// <summary>
	/// -
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector2 operator-(const Vector2& v);

	/// <summary>
	/// -=
	/// </summary>
	/// <param name="v"></param>
	void operator-=(const Vector2& v);

	/// <summary>
	/// *
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	Vector2 operator*(float v);

	/// <summary>
	/// *=
	/// </summary>
	/// <param name="v"></param>
	void operator*=(float v);

};

inline void to_json(nlohmann::json& json, const Vector2& value) {
	json = nlohmann::json::array({ value.x, value.y });
}

inline void from_json(const nlohmann::json& json, Vector2& value) {
	if (json.is_array() && json.size() == 2) {
		// float型のjson配列登録
		value = { json.at(0), json.at(1) };
	}
}
