#include "WinApp.h"
#pragma comment(lib, "winmm.lib")

//#ifdef _DEBUG
#include "../../externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//#endif

const wchar_t WinApp::kWindowClassName_[] = L"DirectXGame";

//	シングルトンインスタンスの取得
WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破壊された
	case WM_DESTROY:
		//OSにたいして、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

// ゲームウィンドウの作成
void WinApp::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight) {

	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	windowStyle_ = windowStyle;

	//ウィンドウプロシージャ
	wndClass_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wndClass_.lpszClassName = kWindowClassName_;
	//インスタンスハンドル
	wndClass_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wndClass_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0, 0, clientWidth, clientHeight };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, windowStyle_, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wndClass_.lpszClassName,		//利用するクラス名
		title,					//タイトルバーの文字
		windowStyle_,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(WindowsOSに任せる)
		CW_USEDEFAULT,			//表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wndClass_.hInstance,			//インスタンスハンドル
		nullptr);				//オプション

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

#ifdef _RELEASE

	//フルスクリーン
	SetWindowLong(hwnd_, GWL_STYLE, WS_POPUP);
	SetWindowPos(hwnd_, HWND_TOP, 0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	//マウスカーソル非表示
	int mouseCounter=0;
	do
	{
		mouseCounter = ShowCursor(false);
	} while(mouseCounter>0);

#endif // _RELEASE

	// システムタイマーの分解度を上げる
	timeBeginPeriod(1);

}

// ゲームウィンドウの破棄
void WinApp::TerminateGameWindow() {

	CloseWindow(hwnd_);

	//ゲーム終了時にCOMの終了処理を行っておく
	CoUninitialize();

}

// メッセージの処理
bool WinApp::ProcessMessage() {
	
	MSG msg{};
	//Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;

}
