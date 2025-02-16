#pragma once
#include <array>

#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

#include "../../Camera/FollowCamera.h"
#include "../../Effect/EffectManager.h"
#include "../../Object/Player/Player.h"
#include "../../Collision/GameSceneCollisionManager.h"
#include "../../UI/UIManager.h"
#include "../../System/Start/StartCountDown.h"
#include "../../System/PostEffectSystem/PostEffectSystem.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : 
	public IScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	// パーティクルモデル
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<GameSceneCollisionManager> collisionManager_;

	// 追加カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;

	// UIマネージャー
	std::unique_ptr<UIManager> uiManager_ = nullptr;

	// カウントダウンシステム
	std::unique_ptr<StartCountDown> startCountDown_ = nullptr;

	// ポストエフェクトシステム
	std::unique_ptr<PostEffectSystem> postEffectSystem_ = nullptr;

};
