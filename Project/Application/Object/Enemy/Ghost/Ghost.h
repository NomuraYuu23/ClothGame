#pragma once
#include "../BaseEnemy.h"
#include "Animation/GhostAnimation.h"
#include "State/System/GhostStateSystem.h"

class Ghost :
    public BaseEnemy
{

public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Ghost();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Ghost() override;

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
    void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) override;

private: // メンバ変数

    // ステートシステム
    std::unique_ptr<GhostStateSystem> ghostStateSystem_;

    // アニメーション
   std::unique_ptr<GhostAnimation> ghostAnimation_;

};

