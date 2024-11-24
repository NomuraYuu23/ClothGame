#include "GhostStateFactory.h"

#include "../GhostStateRoot.h"
#include "../GhostStateSideMove.h"

IGhostState* GhostStateFactory::CreateGhostState(uint32_t ghostStateName)
{
	// 次のステートを生成
	IGhostState* newGhostState = nullptr;

	switch (ghostStateName)
	{
	case kGhostStateIndexRoot: // 通常
		newGhostState = new GhostStateRoot();
		break;
	case kGhostStateIndexSideMove: // 通常
		newGhostState = new GhostStateSideMove();
		break;
	case kGhostStateIndexOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newGhostState;

}