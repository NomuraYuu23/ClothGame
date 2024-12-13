#include "StickLUI.h"

void StickLUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

}

void StickLUI::Update()
{

	BaseUI::Update();

	// 位置に加算する値
	Vector2 addPosition = { 0.0f,0.0f };

	// スティックの値を割る値(移動距離調整)
	const float kDenominator = 3276.7f;
	addPosition.x = input_->GetLeftAnalogstick().x / kDenominator;

	sprite_->SetPosition(position_ + addPosition);

}

void StickLUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}
