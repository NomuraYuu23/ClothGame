#pragma once
#include "../../../BaseCharacter/BaseCharacterAnimation.h"

class GhostAnimation :
    public BaseCharacterAnimation
{

public: // サブクラス

	// モーション番号
	enum GhostMotionIndex {
		kGhostMotionIndexWait, // 待ち
		kGhostMotionIndexOfCount // 数
	};

};

