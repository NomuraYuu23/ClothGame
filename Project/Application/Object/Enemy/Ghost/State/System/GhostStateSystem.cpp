#include "GhostStateSystem.h"

void GhostStateSystem::Initialize(Ghost* ghost)
{

	// ステート
	ghostState_.reset(GhostStateFactory::CreateGhostState(GhostStateIndex::kGhostStateIndexRoot)); // 最初のステート
	ghostState_->Initialize();

	// ステート番号
	currentStateNo_ = GhostStateIndex::kGhostStateIndexRoot; // 最初のステート
	prevStateNo_ = GhostStateIndex::kGhostStateIndexRoot; // 最初のステート
	nextStateNo_ = GhostStateIndex::kGhostStateIndexRoot; // 最初のステート
	ghostState_->SetGhost(ghost); // プレイヤー設定
	ghostState_->SetGhostStateSystem(this); // プレイヤーステートシステム設定

}

void GhostStateSystem::Update()
{

	currentStateNo_ = ghostState_->GetGhostStateNo();

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		ghostState_.reset(GhostStateFactory::CreateGhostState(currentStateNo_));
		ghostState_->Initialize();
	}

	// ステート更新
	ghostState_->Update();

}
