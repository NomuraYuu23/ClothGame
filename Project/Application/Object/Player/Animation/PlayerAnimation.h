#pragma once
#include "../../BaseCharacter/BaseCharacterAnimation.h"

class PlayerAnimation : 
	public BaseCharacterAnimation
{

public: // サブクラス

	// モーション番号
	enum PlayerMotionIndex {
		kPlayerMotionIndexWait, // 待ち
		kPlayerMotionIndexOfCount // 数
	};

};

