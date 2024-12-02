#include "StickLUI.h"

void StickLUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	BaseUI::Initialize(textureHandle, position, size);

}

void StickLUI::Update()
{

	BaseUI::Update();


	Vector2 addPosition = { 0.0f,0.0f };

	const float kDenominator = 3276.7f;
	addPosition.x = input_->GetLeftAnalogstick().x / kDenominator;

	sprite_->SetPosition(position_ + addPosition);

}

void StickLUI::ImGuiDraw()
{

	BaseUI::ImGuiDraw();

}
