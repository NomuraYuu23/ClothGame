#pragma once
#include "../../../Engine/Scene/SceneManager/BaseSceneManager.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager :
    public BaseSceneManager
{

public: // メンバ関数

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="earlySceneNo">最初のシーン</param>
    void Initialize(uint32_t earlySceneNo) override;

};

