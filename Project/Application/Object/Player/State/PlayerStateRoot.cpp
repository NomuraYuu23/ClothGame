#include "PlayerStateRoot.h"
#include "../Player.h"

void PlayerStateRoot::Initialize()
{

}

void PlayerStateRoot::Update()
{

	// 移動関数
	Move();

	// コマンドを受け取る
	playerStateSystem_->SetReceiveCommand(true);

}
