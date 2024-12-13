#include "BaseUI.h"
#include "../../Engine/2D/ImguiManager.h"

Input* BaseUI::input_ = Input::GetInstance();

void BaseUI::Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size)
{

	// 白
	const Vector4 kWhite = { 1.0f,1.0f,1.0f, 1.0f };

	// スプライト
	sprite_.reset(Sprite::Create(textureHandle, position, kWhite));
	sprite_->SetSize(size);

	// 位置
	position_ = position;

	// 大きさ
	size_ = sprite_->GetSize();

	// テクスチャ大きさ(どこまで映すか)
	textureSize_ = sprite_->GetTextureInitSize();

	// 左上
	textureLeftTop_ = sprite_->GetTextureLeftTop();

}

void BaseUI::Draw()
{

	// スプライト
	sprite_->Draw();

}

void BaseUI::ImGuiDraw()
{

	const float kImGuiSpeed = 0.05f;
	ImGui::DragFloat2("Position", &position_.x, kImGuiSpeed);
	ImGui::DragFloat2("Size", &size_.x, kImGuiSpeed);
	ImGui::DragFloat2("TextureSize", &textureSize_.x, kImGuiSpeed);
	ImGui::DragFloat2("TextureLeftTop", &textureLeftTop_.x, kImGuiSpeed);

	// 更新
	sprite_->SetPosition(position_);
	sprite_->SetSize(size_);
	sprite_->SetTextureSize(textureSize_);
	sprite_->SetTextureLeftTop(textureLeftTop_);

}
