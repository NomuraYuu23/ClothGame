#pragma once
#include <Windows.h>
#include <cstdint>

/// <summary>
/// ウインドウアプリケーション
/// </summary>
class WinApp
{

public: // 定数
	
	// ウィンドウ幅
	static const int kWindowWidth_ = 1280;
	// ウインドウ高さ
	static const int kWindowHeight_ = 720;
	//ウィンドウクラス名
	static const wchar_t kWindowClassName_[];

public: // メンバ関数

	/// <summary>
	///	シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
		WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	/// <param name="title">ウィンドウタイトル</param>
	/// <param name="windowStyle">ウィンドウの初期スタイル</param>
	/// <param name="clientWidth">ウィンドウのクライアント領域の初期幅</param>
	/// <param name="clientHeight">ウィンドウのクライアント領域の初期高さ</param>
	void CreateGameWindow(const wchar_t* title = L"DirectXGame",
		UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth_, int32_t clientHeight = kWindowHeight_);

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; }

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() const { return wndClass_.hInstance; }

private: // シングルトン
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private: // メンバ変数

	HWND hwnd_ = nullptr;//ウィンドウハンドル
	WNDCLASS wndClass_{};//ウィンドウクラス
	UINT windowStyle_;//ウィンドウスタイル

};

