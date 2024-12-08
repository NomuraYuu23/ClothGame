#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../Collider/CollisionConfig.h"

class BaseEnemy :
    public MeshObject
{

public: // メンバ関数

    /// <summary>
    /// コンストラクタ
    /// </summary>
    BaseEnemy();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~BaseEnemy() override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    virtual void Initialize(LevelData::MeshData* data) override;

    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="colliderPartner"></param>
    /// <param name="collisionData"></param>
    virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) = 0;

protected: // メンバ関数

    /// <summary>
    /// コライダー更新
    /// </summary>
    virtual void ColliderUpdate();

public: // アクセッサ

    /// <summary>
    /// プレイヤーと衝突するか取得
    /// </summary>
    /// <returns></returns>
    bool GetConflictWithPlayer() { return conflictWithPlayer_; }

    /// <summary>
    /// プレイヤーと衝突するか設定
    /// </summary>
    /// <param name="conflictWithPlayer"></param>
    void SetConflictWithPlayer(bool conflictWithPlayer) { conflictWithPlayer_ = conflictWithPlayer; }

protected: // メンバ変数

    // プレイヤーと衝突するか
    bool conflictWithPlayer_;

};

