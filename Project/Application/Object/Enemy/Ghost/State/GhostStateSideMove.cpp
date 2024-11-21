#include "GhostStateSideMove.h"
#include "../Ghost.h"
#include "../../../Engine/Math/Ease.h"
#include <cmath>

void GhostStateSideMove::Initialize()
{

	// 状態番号変更
	ghostStateNo_ = kGhostStateIndexSideMove;

	// ラジアン角
	radian_ = 0.0f;

}

void GhostStateSideMove::Update()
{

	// ワールドトランスフォーム取得
	WorldTransform* worldTransform = ghost_->GetWorldTransformAdress();

	// ラジアン角
	const float kRadianSpeed = 0.003f;
	radian_ = std::fmodf(radian_ + kRadianSpeed, 3.14f);

	// tを三角関数で
	float t = std::sinf(radian_);
	const float positionMinX = -7.0f;
	const float positionMaxX = 7.0f;
	// 移動
	worldTransform->transform_.translate.x = Ease::Easing(Ease::EaseName::EaseInOutQuad, positionMinX, positionMaxX, t);

}
