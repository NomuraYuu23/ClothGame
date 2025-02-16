#pragma once

#include "../BaseSceneTransition/BaseSceneTransition.h"

/// <summary>
/// シーン遷移の基盤
/// </summary>
class AbstractSceneTransitionFactory
{

public: //メンバ関数

	// 仮想デストラクタ
	virtual ~AbstractSceneTransitionFactory() = default;
	// シーン生成
	virtual BaseSceneTransition* CreateSceneTransition(int sceneName, int requestSeneName) = 0;

};

