#pragma once
#include <string>

/// <summary>
/// ログ
/// </summary>
class Log
{

public: // メンバ関数

	/// <summary>
	/// ログ
	/// </summary>
	/// <param name="message">メッセージ</param>
	static void Message(const std::string& message);

	/// <summary>
	/// コンバートストリング
	/// </summary>
	/// <param name="message">メッセージ</param>
	static std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// コンバートストリング
	/// </summary>
	/// <param name="message">メッセージ</param>
	static std::string ConvertString(const std::wstring& str);

private: // シングルトン
	Log() = default;
	~Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;

};

