#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Animation/Animation.h"
#include "State/IPlayerState.h"
#include "State/System/PlayerStateFactory.h"
#include "State/System/PlayerStateSystem.h"
#include "Animation/PlayerAnimation.h"

class Player :
    public MeshObject
{

public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Player();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Player() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void Draw(BaseCamera& camera) override;

    /// <summary>
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// パーティクル描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    void ParticleDraw(BaseCamera& camera) override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private: // メンバ関数

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

private: // メンバ変数

    // ステートシステム
    std::unique_ptr<PlayerStateSystem> playerStateSystem_;

    // アニメーション
    std::unique_ptr<PlayerAnimation> playerAnimation_;

};

