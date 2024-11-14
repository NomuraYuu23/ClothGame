#pragma once

#include <Windows.h>
#include <DbgHelp.h>
#include <shlobj.h>
#include <strsafe.h>
#include <tchar.h>

#include <iostream>

// クラッシュハンドラー
class CrashHandler
{

public: // 関数

	/// <summary>
	///	ダンプファイル作成
	/// </summary>
	/// <param name="expPtr"></param>
	/// <returns></returns>
	static int GenerateDump(EXCEPTION_POINTERS* expPtr);

};

