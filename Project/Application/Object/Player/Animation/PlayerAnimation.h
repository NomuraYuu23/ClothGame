#pragma once
#include "../../BaseCharacter/BaseCharacterAnimation.h"

class PlayerAnimation : 
	public BaseCharacterAnimation
{

public: // サブクラス

	// モーション番号
	enum PlayerMotionIndex {
		kPlayerMotionIndexRun, // 走る
		kPlayerMotionIndexJump, // ジャンプ
		kPlayerMotionIndexDash, // ダッシュ
		kPlayerMotionIndexDamage, // ダメージ
		kPlayerMotionIndexOfCount // 数
	};

};

