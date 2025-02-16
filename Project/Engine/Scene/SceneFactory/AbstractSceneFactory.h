#pragma once

#include "../BaseScene/BaseScene.h"

/// <summary>
/// シーンファクトリーの基盤
/// </summary>
class AbstractSceneFactory {

public: //メンバ関数

	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual BaseScene* CreateScene(int sceneName) = 0;

};
