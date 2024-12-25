#pragma once
#include "../../../Engine/SceneTransition/ISceneTransition/ISceneTransition.h"
#include <memory>
#include "../../../Engine/2D/Sprite.h"
#include "../../../Engine/Camera/BaseCamera.h"
#include "../../OutGameObject/Curtain/Curtain.h"

/// <summary>
/// シーン遷移カーテンver
/// </summary>
class SceneTransitionCurtain :
    public ISceneTransition
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneTransitionCurtain() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: 

	// カメラ
	BaseCamera camera_;

	// カーテン
	std::unique_ptr<Curtain> curtain_;

};

