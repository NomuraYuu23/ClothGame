#include "PlayerStateFactory.h"
#include "../PlayerStateRoot.h"
#include "../PlayerStateJump.h"
#include "../PlayerStateDamage.h"
#include "../PlayerStateFall.h"
#include "../PlayerStateDash.h"

IPlayerState* PlayerStateFactory::CreatePlayerState(uint32_t playerStateName)
{
	// 次のステートを生成
	IPlayerState* newPlayerState = nullptr;

	switch (playerStateName)
	{
	case kPlayerStateIndexRoot: // 通常
		newPlayerState = new PlayerStateRoot();
		break;
	case kPlayerStateIndexJump: // ジャンプ
		newPlayerState = new PlayerStateJump();
		break;
	case kPlayerStateIndexDamage: // ダメージ
		newPlayerState = new PlayerStateDamage();
		break;
	case kPlayerStateIndexFall: // 落下
		newPlayerState = new PlayerStateFall();
		break;	
	case kPlayerStateIndexDash: // ダッシュ
		newPlayerState = new PlayerStateDash();
		break;
	case kPlayerStateIndexOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
	
}
