#include "ButtonAUI.h"

void ButtonAUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

}

void ButtonAUI::Update()
{

	BaseUI::Update();

	// 押したときに色が変わる
	const Vector4 kWhite = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 kGray = { 0.5f,0.5f,0.5f,1.0f };

	if (input_->PushJoystick(JoystickButton::kJoystickButtonA)) {
		sprite_->SetColor(kGray);
	}
	else {
		sprite_->SetColor(kWhite);
	}

}

void ButtonAUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}
