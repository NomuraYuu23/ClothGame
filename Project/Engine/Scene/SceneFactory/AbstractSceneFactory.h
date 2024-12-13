#pragma once

#include "../IScene/IScene.h"

/// <summary>
/// シーンファクトリーの基盤
/// </summary>
class AbstractSceneFactory {

public: //メンバ関数

	// 仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	// シーン生成
	virtual IScene* CreateScene(int sceneName) = 0;

};
