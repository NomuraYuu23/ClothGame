#pragma once

#include <Windows.h>
#include <DbgHelp.h>
#include <shlobj.h>
#include <strsafe.h>
#include <tchar.h>

#include <iostream>

/// <summary>
/// クラッシュハンドラー
/// </summary>
class CrashHandler
{

public: // メンバ関数

	/// <summary>
	///	ダンプファイル作成
	/// </summary>
	/// <param name="expPtr">ポインタ</param>
	/// <returns></returns>
	static int GenerateDump(EXCEPTION_POINTERS* expPtr);

};

