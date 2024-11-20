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

};

