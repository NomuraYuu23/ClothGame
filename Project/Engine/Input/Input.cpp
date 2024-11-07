#include "Input.h"
#include <cassert>
#include <cmath>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

//インスタンス
Input* Input::GetInstance() {

	static Input instance;

	return &instance;

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hwnd"></param>
void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

	// DirectInputの初期化
	directInput_ = nullptr;
	HRESULT result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &directKeyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力デ―タ形式のセット
	result = directKeyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = directKeyboard_->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成
	result = directInput_->CreateDevice(GUID_SysMouse, &directMouse_, NULL);
	assert(SUCCEEDED(result));

	// 入力デ―タ形式のセット
	result = directMouse_->SetDataFormat(&c_dfDIMouse2);//標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = directMouse_->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// ジョイスティックの生成
	JoystickConnected(hwnd);

}

/// <summary>
/// 毎フレーム
/// </summary>
void Input::Update() {

	//キーボード関連更新
	KeyboardUpdate();

	//マウス関連更新
	MouseUpdate();

	//ジョイスティック関連更新
	if (joystickConnected) {
		JoystickUpdate();
	}

}

/// <summary>
/// キーボード関連更新
/// </summary>
void Input::KeyboardUpdate() {

	//キーボード動作開始
	directKeyboard_->Acquire();

	//前回のキー入力を保存
	keyPre_ = key_;

	// 全キーの入力状態を取得する
	directKeyboard_->GetDeviceState((DWORD)size(key_), key_.data());


}

/// <summary>
/// キーを押した状態か
/// </summary>
/// <param name="keyNumber">キー番号</param>
/// <returns>キーを押した状態か</returns>
bool Input::PushKey(uint8_t keyNumber) const {

	//0でなければ押している
	if (key_[keyNumber]) {
		return true;
	}

	//押していない
	return false;

}

/// <summary>
/// キーを離した状態か
/// </summary>
/// <param name="keyNumber">キー番号</param>
/// <returns>キーを離した状態か</returns>
bool Input::NoPushKey(uint8_t keyNumber) const {

	//0でなければ押している
	if (key_[keyNumber]) {
		return false;
	}

	//押していない
	return true;


}

/// <summary>
/// キーを押した瞬間か
/// </summary>
/// <param name="keyNumber">キー番号</param>
/// <returns>キーを押した瞬間か</returns>
bool Input::TriggerKey(uint8_t keyNumber) const {

	//前回が0で、今回が0でなければtrue
	if (!keyPre_[keyNumber] && key_[keyNumber]) {
		return true;
	}

	// false
	return false;

}

/// <summary>
/// キーを離した瞬間か
/// </summary>
/// <param name="keyNumber">キー番号</param>
/// <returns>キーを離した瞬間か</returns>
bool Input::ReleaseKey(uint8_t keyNumber) const {

	//前回が0でなく、今回が0でならtrue
	if (keyPre_[keyNumber] && !key_[keyNumber]) {
		return true;
	}

	// false
	return false;

}


/// <summary>
/// マウス関連更新
/// </summary>
void Input::MouseUpdate() {

	// マウス動作開始
	directMouse_->Acquire();

	//前回のマウス入力を保存
	mousePre_ = mouse_;

	// マウスの入力状態を取得する
	directMouse_->GetDeviceState(sizeof(mouse_), &mouse_);

}

/// <summary>
/// マウスボタンを押した状態か
/// </summary>
/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>押されているか</returns>
bool Input::PushMouse(uint8_t mouseNumber) const {

	// クリックされているか
	if (mouse_.rgbButtons[mouseNumber]) {
		return true;
	}

	return false;

}


/// <summary>
/// マウスボタンを離した状態か
/// </summary>
/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>マウスボタンを離した状態か</returns>
bool Input::NoPushMouse(uint8_t mouseNumber) const {

	// クリックされているか
	if (mouse_.rgbButtons[mouseNumber]) {
		return false;
	}

	return true;

}

/// <summary>
/// マウスボタンを押した瞬間か
/// </summary>
/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>マウスボタンを押した瞬間か</returns>
bool Input::TriggerMouse(uint8_t mouseNumber) const {

	//クリックした瞬間
	if (mouse_.rgbButtons[mouseNumber] && !mousePre_.rgbButtons[mouseNumber]) {
		return true;
	}

	return false;

}

/// <summary>
/// マウスボタンを離した瞬間か
/// </summary>
/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
/// <returns>マウスボタンを離した瞬間か</returns>
bool Input::ReleaseMouse(uint8_t mouseNumber) const {

	//クリックやめた瞬間
	if (!mouse_.rgbButtons[mouseNumber] && mousePre_.rgbButtons[mouseNumber]) {
		return true;
	}

	return false;

}

/// <summary>
/// マウス移動量を取得
/// </summary>
/// <returns>マウス移動量</returns>
Input::MouseMove Input::GetMouseMove() {

	return Input::MouseMove(mouse_.lX, mouse_.lY, mouse_.lZ);

}

/// <summary>
/// マウスの位置を取得する（ウィンドウ座標系）
/// </summary>
/// <returns>マウスの位置</returns>
const Vector2& Input::GetMousePosition(HWND hwnd) {

	//マウス座標(スクリーン座標)を取得する
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);
	//クライアントエリア座標に変換する
	ScreenToClient(hwnd, &mousePosition);

	mousePosition_.x = float(mousePosition.x);
	mousePosition_.y = float(mousePosition.y);

	return mousePosition_;

}

void Input::JoystickUpdate() {
	
	if (usedXInput_) {
		XJoystickUpdate();
	}
	else {
		DirectJoystickUpdate();
	}

}

bool Input::PushJoystick(uint8_t joystickNumber) {
	
	if (usedXInput_) {
		return XPushJoystick(joystickNumber);
	}
	else {
		return DirectPushJoystick(joystickNumber);
	}

}

bool Input::NoPushJoystick(uint8_t joystickNumber) {
	
	if (usedXInput_) {
		return XNoPushJoystick(joystickNumber);
	}
	else {
		return DirectNoPushJoystick(joystickNumber);
	}

}

bool Input::TriggerJoystick(uint8_t joystickNumber) {
	
	if (usedXInput_) {
		return XTriggerJoystick(joystickNumber);
	}
	else {
		return DirectTriggerJoystick(joystickNumber);
	}

}

bool Input::ReleaseJoystick(uint8_t joystickNumber) {
	
	if (usedXInput_) {
		return XReleaseJoystick(joystickNumber);
	}
	else {
		return DirectReleaseJoystick(joystickNumber);
	}

}

Vector2 Input::GetLeftAnalogstick(float deadZone) {
	
	if (usedXInput_) {
		return XGetLeftAnalogstick(deadZone);
	}
	else {
		return DirectGetLeftAnalogstick(deadZone);
	}

}

Vector2 Input::GetRightAnalogstick(float deadZone) {
	
	if (usedXInput_) {
		return XGetRightAnalogstick(deadZone);
	}
	else {
		return DirectGetRightAnalogstick(deadZone);
	}

}

float Input::GetLRTrrigger() {

	if (usedXInput_) {
		// 出来てない
		assert(0);
		return 0.0f;
	}
	else {
		return DirectGetLRTrrigger();
	}

}

bool Input::GetLTrrigger()
{

	if (usedXInput_) {
		return XGetLTrrigger();
	}
	else {
		// 出来てない
		assert(0);
		return false;
	}

}

bool Input::GetRTrrigger()
{

	if (usedXInput_) {
		return XGetRTrrigger();
	}
	else {
		// 出来てない
		assert(0);
		return false;
	}

}

void Input::JoystickConnected(HWND hwnd) {

	if (usedXInput_) {
		XJoystickConnected();
	}
	else {
		DirectJoystickConnected(hwnd);
	}

}

void Input::XJoystickConnected()
{

	ZeroMemory(&xJoystickState_, sizeof(XINPUT_STATE));

	xJoystick_ = XInputGetState(0, &xJoystickState_);
	
	if(xJoystick_ == ERROR_SUCCESS) {
		joystickConnected = true;
	}
	else {
		joystickConnected = false;
	}

}

void Input::XJoystickUpdate()
{

	xJoystickPreState_ = xJoystickState_;

	XJoystickConnected();

}

bool Input::XPushJoystick(uint8_t joystickNumber)
{

	uint32_t button = XInputButtonConverter(joystickNumber);

	if (xJoystickState_.Gamepad.wButtons & button) {
		return true;
	}

	return false;

}

bool Input::XNoPushJoystick(uint8_t joystickNumber)
{

	uint32_t button = XInputButtonConverter(joystickNumber);

	if (!(xJoystickState_.Gamepad.wButtons & button)) {
		return true;
	}

	return false;

}

bool Input::XTriggerJoystick(uint8_t joystickNumber)
{

	uint32_t button = XInputButtonConverter(joystickNumber);

	if ((xJoystickState_.Gamepad.wButtons & button) && 
		!(xJoystickPreState_.Gamepad.wButtons & button)) {
		return true;
	}

	return false;

}

bool Input::XReleaseJoystick(uint8_t joystickNumber)
{

	uint32_t button = XInputButtonConverter(joystickNumber);

	if (!(xJoystickState_.Gamepad.wButtons & button) &&
		(xJoystickPreState_.Gamepad.wButtons & button)) {
		return true;
	}

	return false;

}

Vector2 Input::XGetLeftAnalogstick(float deadZone)
{

	Vector2 sThumbL = { float(xJoystickState_.Gamepad.sThumbLX), float(-xJoystickState_.Gamepad.sThumbLY) };

	if (sThumbL.x < deadZone &&
		sThumbL.x > -deadZone) {
		sThumbL.x = 0.0f;
	}
	if (sThumbL.y < deadZone &&
		sThumbL.y > -deadZone) {
		sThumbL.y = 0.0f;
	}

	return sThumbL;

}

Vector2 Input::XGetRightAnalogstick(float deadZone)
{

	Vector2 sThumbR = { float(xJoystickState_.Gamepad.sThumbRX), float(-xJoystickState_.Gamepad.sThumbRY) };

	if (sThumbR.x < deadZone &&
		sThumbR.x > -deadZone) {
		sThumbR.x = 0.0f;
	}
	if (sThumbR.y < deadZone &&
		sThumbR.y > -deadZone) {
		sThumbR.y = 0.0f;
	}

	return sThumbR;

}

bool Input::XGetLTrrigger()
{
	
	if (xJoystickState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}

	return false;

}

bool Input::XGetRTrrigger()
{

	if (xJoystickState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		return true;
	}

	return false;

}

uint32_t Input::XInputButtonConverter(uint8_t joystickNumber)
{
	
	JoystickButton button = static_cast<JoystickButton>(joystickNumber);

	uint32_t result = 0u;

	switch (button)
	{
	case kJoystickButtonA:
		result = 0x1000;
		break;
	case kJoystickButtonB:
		result = 0x2000;
		break;
	case kJoystickButtonX:
		result = 0x4000;
		break;
	case kJoystickButtonY:
		result = 0x8000;
		break;
	case kJoystickButtonLB:
		result = 0x0100;
		break;
	case kJoystickButtonRB:
		result = 0x0200;
		break;
	case kJoystickButtonBACK:
		result = 0x0020;
		break;
	case kJoystickButtonSTART:
		result = 0x0010;
		break;
	case kJoystickButtonLST:
		result = 0x0040;
		break;
	case kJoystickButtonRST:
		result = 0x0080;
		break;
	default:
		break;
	}

	return result;

}

void Input::DirectJoystickConnected(HWND hwnd)
{

	// ジョイスティックの生成
	HRESULT result = directInput_->CreateDevice(GUID_Joystick, &directJoystick_, NULL);
	if (SUCCEEDED(result)) {

		// 入力デ―タ形式のセット
		result = directJoystick_->SetDataFormat(&c_dfDIJoystick2);//標準形式
		assert(SUCCEEDED(result));

		// 排他制御レベルのセット
		result = directJoystick_->SetCooperativeLevel(
			hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));

		joystickConnected = true;

	}
	else {
		joystickConnected = false;
	}

}

void Input::DirectJoystickUpdate()
{
	//ジョイスティック動作開始
	directJoystick_->Acquire();

	//前回のジョイスティック入力を保存
	directJoystickPreState_ = directJoystickState_;

	// ジョイスティックの入力状態を取得する
	directJoystick_->GetDeviceState(sizeof(directJoystickState_), &directJoystickState_);
}

bool Input::DirectPushJoystick(uint8_t joystickNumber) const
{
	if (!joystickConnected) {
		return false;
	}

	if (directJoystickState_.rgbButtons[joystickNumber]) {
		return true;
	}

	return false;
}

bool Input::DirectNoPushJoystick(uint8_t joystickNumber) const
{
	if (!joystickConnected) {
		return false;
	}

	if (directJoystickState_.rgbButtons[joystickNumber]) {
		return false;
	}

	return true;
}

bool Input::DirectTriggerJoystick(uint8_t joystickNumber) const
{
	if (!joystickConnected) {
		return false;
	}

	if (directJoystickState_.rgbButtons[joystickNumber] && !directJoystickPreState_.rgbButtons[joystickNumber]) {
		return true;
	}

	return false;
}

bool Input::DirectReleaseJoystick(uint8_t joystickNumber) const
{
	if (!joystickConnected) {
		return false;
	}

	if (!directJoystickState_.rgbButtons[joystickNumber] && directJoystickPreState_.rgbButtons[joystickNumber]) {
		return true;
	}

	return false;
}

Vector2 Input::DirectGetLeftAnalogstick(float deadZone) const
{
	if (!joystickConnected) {
		return Vector2(0.0f, 0.0f);
	}

	Vector2 result = { float(directJoystickState_.lX) - 32767.0f, float(directJoystickState_.lY) - 32767.0f };

	if (std::fabsf(result.x) < deadZone) {
		result.x = 0.0f;
	}
	if (std::fabsf(result.y) < deadZone) {
		result.y = 0.0f;
	}

	return result;
}

Vector2 Input::DirectGetRightAnalogstick(float deadZone) const
{
	if (!joystickConnected) {
		return Vector2(0.0f, 0.0f);
	}

	Vector2 result = { float(directJoystickState_.lRx) - 32767.0f, float(directJoystickState_.lRy) - 32767.0f };

	if (std::fabsf(result.x) < deadZone) {
		result.x = 0.0f;
	}
	if (std::fabsf(result.y) < deadZone) {
		result.y = 0.0f;
	}

	return result;

}

float Input::DirectGetLRTrrigger() const
{
	if (!joystickConnected) {
		return 0.0f;
	}

	return float(directJoystickState_.lZ);
}
