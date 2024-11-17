#include "PlayerStateFactory.h"
#include "../PlayerStateRoot.h"
#include "../PlayerStateJump.h"

PlayerStateFactory* PlayerStateFactory::GetInstance()
{
	static PlayerStateFactory instance;
	return &instance;
}

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
	case kPlayerStateIndexOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
	
}
