#include "PlayerStateSystem.h"

void PlayerStateSystem::Initialize(Player*player)
{

	// ステート
	playerState_.reset(PlayerStateFactory::CreatePlayerState(PlayerStateIndex::kPlayerStateIndexRoot)); // 最初のステート
	playerState_->Initialize();

	// ステート番号
	currentStateNo_ = PlayerStateIndex::kPlayerStateIndexRoot; // 最初のステート
	prevStateNo_ = PlayerStateIndex::kPlayerStateIndexRoot; // 最初のステート
	nextStateNo_ = PlayerStateIndex::kPlayerStateIndexRoot; // 最初のステート
	playerState_->SetPlayer(player); // プレイヤー設定
	playerState_->SetPlayerStateSystem(this); // プレイヤーステートシステム設定

	// コマンド
	playerCommand_ = PlayerCommand::GetInstance();
	playerCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// 割り込みコマンドがあるか
	interruptCommand_ = false;

}

void PlayerStateSystem::Update()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (interruptCommand_) {
		currentStateNo_ = nextStateNo_;
		interruptCommand_ = false;
	}
	else if (receiveCommand_) {
		currentStateNo_ = playerCommand_->Command();
	}
	else {
		currentStateNo_ = playerState_->GetPlaryerStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		playerState_.reset(PlayerStateFactory::CreatePlayerState(currentStateNo_));
		playerState_->Initialize();
	}

	// ステート更新
	playerState_->Update();

}
