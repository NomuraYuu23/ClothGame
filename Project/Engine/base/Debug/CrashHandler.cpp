#include "CrashHandler.h"

#pragma comment(lib, "Dbghelp.lib")

int CrashHandler::GenerateDump(EXCEPTION_POINTERS* expPtr)
{

	TCHAR desktopDir[MAX_PATH];
	SHGetSpecialFolderPath(NULL, desktopDir, CSIDL_DESKTOPDIRECTORY, FALSE);

	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	TCHAR dumpPath[MAX_PATH];
	StringCchPrintf(
		dumpPath, MAX_PATH, _T("%s\\%s-%04d%02d%02d-%02d%02d%02d-%1d-%1d.dmp"),
		desktopDir, _T("dump_test"), localTime.wYear, localTime.wMonth, localTime.wDay,
		localTime.wHour, localTime.wMinute, localTime.wSecond,
		GetCurrentProcessId(), GetCurrentThreadId());

	HANDLE dumpFile =
		CreateFile(dumpPath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	MINIDUMP_EXCEPTION_INFORMATION expInfo;
	expInfo.ThreadId = GetCurrentThreadId();
	expInfo.ExceptionPointers = expPtr;
	expInfo.ClientPointers = TRUE;

	BOOL miniDumpSuccessful =
		MiniDumpWriteDump(
			GetCurrentProcess(), GetCurrentProcessId(), dumpFile,
			MiniDumpWithDataSegs, &expInfo, NULL, NULL);

	// 警告回避
	miniDumpSuccessful;

	return EXCEPTION_EXECUTE_HANDLER;

}
