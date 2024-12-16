#pragma once

#include <array>
#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#include <Xinput.h>

#include "../Math/Vector/Vector2.h"

/// <summary>
/// ジョイスティックボタン一覧
/// </summary>
enum JoystickButton {
	kJoystickButtonA, // ボタンA
	kJoystickButtonB, // ボタンB
	kJoystickButtonX, // ボタンX
	kJoystickButtonY, // ボタンY
	kJoystickButtonLB, // ボタンLB
	kJoystickButtonRB, // ボタンRB
	kJoystickButtonBACK, // ボタンBACK
	kJoystickButtonSTART, // ボタンSTART
	kJoystickButtonLST, // ボタンLST
	kJoystickButtonRST, // ボタンRST
};

/// <summary>
/// 入力関係
/// </summary>
class Input
{

public:

	/// <summary>
	/// マウスがどれだけ動いたか
	/// </summary>
	struct MouseMove {
		LONG lX; // X
		LONG lY; // Y
		LONG lZ; // Z
	};

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hInstance"></param>
	/// <param name="hwnd"></param>
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// 毎フレーム
	/// </summary>
	void Update();

	/// <summary>
	/// キーボード関連更新
	/// </summary>
	void KeyboardUpdate();

	/// <summary>
	/// キーを押した状態か
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>キーを押した状態か</returns>
	bool PushKey(uint8_t keyNumber) const;

	/// <summary>
	/// キーを離した状態か
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>キーを離した状態か</returns>
	bool NoPushKey(uint8_t keyNumber) const;

	/// <summary>
	/// キーを押した瞬間か
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>キーを押した瞬間か</returns>
	bool TriggerKey(uint8_t keyNumber) const;

	/// <summary>
	/// キーを離した瞬間か
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>キーを離した瞬間か</returns>
	bool ReleaseKey(uint8_t keyNumber) const;

	/// <summary>
	/// キー取得
	/// </summary>
	/// <returns></returns>
	const std::array<BYTE, 256>& GetAllKey() const { return key_; }

	/// <summary>
	/// マウス関連更新
	/// </summary>
	void MouseUpdate();

	/// <summary>
	/// マウスボタンを押した状態か
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>マウスボタンを押した状態か</returns>
	bool PushMouse(uint8_t mouseNumber) const;

	/// <summary>
	/// マウスボタンを離した状態か
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>マウスボタンを離した状態か</returns>
	bool NoPushMouse(uint8_t mouseNumber) const;

	/// <summary>
	/// マウスボタンを押した瞬間か
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>マウスボタンを押した瞬間か</returns>
	bool TriggerMouse(uint8_t mouseNumber) const;

	/// <summary>
	/// マウスボタンを離した瞬間か
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>マウスボタンを離した瞬間か</returns>
	bool ReleaseMouse(uint8_t mouseNumber) const;

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// マウスの位置を取得する（ウィンドウ座標系）
	/// </summary>
	/// <returns>マウスの位置</returns>
	const Vector2& GetMousePosition(HWND hwnd);

	/// <summary>
	/// マウス取得
	/// </summary>
	/// <returns></returns>
	const DIMOUSESTATE2& GetAllMouse() const { return mouse_; }

	/// <summary>
	/// ジョイスティック関連更新
	/// </summary>
	void JoystickUpdate();

	/// <summary>
	/// ジョイスティックボタンを押した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した状態か</returns>
	bool PushJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを離した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した状態か</returns>
	bool NoPushJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを押した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した瞬間か</returns>
	bool TriggerJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを離した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した瞬間か</returns>
	bool ReleaseJoystick(uint8_t joystickNumber);

	/// <summary>
	/// 左のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns></returns>
	Vector2 GetLeftAnalogstick(float deadZone = static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));

	/// <summary>
	/// 右のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns></returns>
	Vector2 GetRightAnalogstick(float deadZone = static_cast<float>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));

	/// <summary>
	/// 左右のトリガーの状態を取得()
	/// </summary>
	/// <returns></returns>
	float GetLRTrrigger();

	/// <summary>
	/// 左のトリガーの状態を取得()
	/// </summary>
	/// <returns></returns>
	bool GetLTrrigger();

	/// <summary>
	/// 右のトリガーの状態を取得()
	/// </summary>
	/// <returns></returns>
	bool GetRTrrigger();

	/// <summary>
	/// ジョイスティック接続
	/// </summary>
	void JoystickConnected(HWND hwnd);

	/// <summary>
	/// つながっているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetJoystickConnected() { return joystickConnected; }

private: //XInput

	/// <summary>
	/// ジョイスティック接続
	/// </summary>
	void XJoystickConnected();

	/// <summary>
	/// ジョイスティック関連更新
	/// </summary>
	void XJoystickUpdate();

	/// <summary>
	/// ジョイスティックボタンを押した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した状態か</returns>
	bool XPushJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを離した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した状態か</returns>
	bool XNoPushJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを押した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した瞬間か</returns>
	bool XTriggerJoystick(uint8_t joystickNumber);

	/// <summary>
	/// ジョイスティックボタンを離した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した瞬間か</returns>
	bool XReleaseJoystick(uint8_t joystickNumber);

	/// <summary>
	/// 左のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns>0~</returns>
	Vector2 XGetLeftAnalogstick(float deadZone);

	/// <summary>
	/// 右のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns></returns>
	Vector2 XGetRightAnalogstick(float deadZone);

	/// <summary>
	/// LTrriger取得
	/// </summary>
	/// <returns></returns>
	bool XGetLTrrigger();
	
	/// <summary>
	/// RTrriger取得
	/// </summary>
	/// <returns></returns>
	bool XGetRTrrigger();

	/// <summary>
	/// Joystickの状態取得
	/// </summary>
	/// <returns></returns>
	const XINPUT_STATE& GetXJoystickState() const { return xJoystickState_; }

	/// <summary>
	/// XInputの入力情報に変換
	/// </summary>
	/// <param name="joystickNumber"></param>
	/// <returns></returns>
	uint32_t XInputButtonConverter(uint8_t joystickNumber);

private: //directInput

	/// <summary>
	/// ジョイスティック接続
	/// </summary>
	void DirectJoystickConnected(HWND hwnd);
	
	/// <summary>
	/// ジョイスティック関連更新
	/// </summary>
	void DirectJoystickUpdate();

	/// <summary>
	/// ジョイスティックボタンを押した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した状態か</returns>
	bool DirectPushJoystick(uint8_t joystickNumber) const;

	/// <summary>
	/// ジョイスティックボタンを離した状態か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した状態か</returns>
	bool DirectNoPushJoystick(uint8_t joystickNumber) const;

	/// <summary>
	/// ジョイスティックボタンを押した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを押した瞬間か</returns>
	bool DirectTriggerJoystick(uint8_t joystickNumber) const;

	/// <summary>
	/// ジョイスティックボタンを離した瞬間か
	/// </summary>
	/// <param name="buttonNumber"></param>
	/// <returns>ジョイスティックボタンを離した瞬間か</returns>
	bool DirectReleaseJoystick(uint8_t joystickNumber) const;

	/// <summary>
	/// 左のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns></returns>
	Vector2 DirectGetLeftAnalogstick(float deadZone) const;
	
	/// <summary>
	/// 右のアナログスティックの状態を取得
	/// </summary>
	/// <param name="deadZone">デッドゾーン</param>
	/// <returns></returns>
	Vector2 DirectGetRightAnalogstick(float deadZone) const;

	/// <summary>
	/// 左右のトリガーの状態を取得
	/// </summary>
	/// <returns></returns>
	float DirectGetLRTrrigger() const;

	/// <summary>
	/// Joystickの状態取得
	/// </summary>
	/// <returns></returns>
	const DIJOYSTATE2& GetDirectJoystickState() const { return directJoystickState_; }

private: // シングルトン

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

private: // メンバ変数

	// DirectInput
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
	
	//キーボード
	Microsoft::WRL::ComPtr<IDirectInputDevice8> directKeyboard_;
	// キー情報
	std::array<BYTE, 256> key_;
	// 前キー情報
	std::array<BYTE, 256> keyPre_;

	// マウス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> directMouse_ = nullptr;
	// マウス状態
	DIMOUSESTATE2 mouse_;
	// マウス前状態
	DIMOUSESTATE2 mousePre_;
	// マウス位置
	Vector2 mousePosition_;

	// directジョイスティック
	Microsoft::WRL::ComPtr<IDirectInputDevice8> directJoystick_ = nullptr;
	// directInput状態
	DIJOYSTATE2 directJoystickState_;
	// directInput前状態
	DIJOYSTATE2 directJoystickPreState_;
	
	// XInputジョイスティック
	DWORD xJoystick_;
	// XInput状態
	XINPUT_STATE xJoystickState_;
	// XInput前状態
	XINPUT_STATE xJoystickPreState_;

	// DIJOYSTATE2かXInputか
	bool usedXInput_ = true;
	//つながっているかどうか
	bool joystickConnected;

};

