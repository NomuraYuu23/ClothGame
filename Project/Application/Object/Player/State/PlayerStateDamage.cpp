#include "PlayerStateDamage.h"
#include "../Player.h"

void PlayerStateDamage::Initialize()
{

	// プレイヤーはジャンプ状態に
	playerStateNo_ = kPlayerStateIndexDamage;

}

void PlayerStateDamage::Update()
{

	// アニメーションの終了時間
	//const float kAnimationEndTime = 0.0f;

	// ダメージアニメーション中

	// アニメーション終了後ワープ 
	
	// ワールドトランスフォーム取得
	playerWorldTransform_ = player_->GetWorldTransformAdress();
	
	// ワープ
	const Vector3 kWarpTranslate = { 0.0f, 4.0f, -470.0f };
	playerWorldTransform_->transform_.translate = kWarpTranslate;
	player_->SetWarping(true);
	
	// 状態をRootへ
	playerStateNo_ = kPlayerStateIndexRoot;

}
