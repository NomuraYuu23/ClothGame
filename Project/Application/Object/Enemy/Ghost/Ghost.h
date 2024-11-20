#pragma once
#include "../BaseEnemy.h"
#include "Animation/GhostAnimation.h"
#include "State/System/GhostStateSystem.h"
#include "../../../../Engine/Physics/ClothGPU/ClothGPU.h"

class Ghost :
    public BaseEnemy
{

public: // メンバ関数

    // 自分の衝突判定データの名前
    static const std::string kMyColliderName_;
    // 大きさ
    static const Vector2 kClothScale_;
    // 分割数
    static const Vector2 kClothDiv_;
    // ワールド座標からの固定部分
    static const Vector3 kBaseFixed_;
    // DirectX
    static DirectXCommon* dxCommon_;

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

    /// <summary>
    /// 布リセット
    /// </summary>
    void ClothReset();

private: // メンバ関数

    /// <summary>
    /// 布初期化
    /// </summary>
    void ClothInitialize();

    /// <summary>
    /// 布更新
    /// </summary>
    void ClothUpdate();

private: // メンバ変数

    // ステートシステム
    std::unique_ptr<GhostStateSystem> ghostStateSystem_;

    // アニメーション
   std::unique_ptr<GhostAnimation> ghostAnimation_;

   // 布
   std::unique_ptr<ClothGPU> cloth_;

   // 自分の衝突判定データ
   ClothGPUCollision::Sphere collider_;

};

