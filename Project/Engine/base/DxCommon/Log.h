#pragma once
#include <string>
class Log
{

public: // 関数

	/// <summary>
	/// ログ
	/// </summary>
	/// <param name="message"></param>
	static void Message(const std::string& message);

	/// <summary>
	/// コンバートストリング
	/// </summary>
	/// <param name="message"></param>
	static std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// コンバートストリング
	/// </summary>
	/// <param name="message"></param>
	static std::string ConvertString(const std::wstring& str);

private:
	Log() = default;
	~Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;

};

