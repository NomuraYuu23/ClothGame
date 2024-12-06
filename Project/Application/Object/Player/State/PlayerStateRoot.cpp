#include "PlayerStateRoot.h"
#include "../Player.h"

void PlayerStateRoot::Initialize()
{

	// プレイヤーは通常状態に
	playerStateNo_ = kPlayerStateIndexRoot;

}

void PlayerStateRoot::Update()
{

	// 移動関数
	Move();

	// 落下に移行
	if (player_->GetFloating()) {
		playerStateSystem_->SetReceiveCommand(false);
		playerStateNo_ = kPlayerStateIndexFall;
		return;
	}

	// コマンドを受け取る
	playerStateSystem_->SetReceiveCommand(true);

}
