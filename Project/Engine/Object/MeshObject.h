#pragma once
#include "IObject.h"
#include "../3D/Model/Model.h"
#include "../Level/LevelData.h"
#include "../Collision/BaseCollisionManager.h"
#include "../Collider/ColliderDebugDraw/ColliderDebugDraw.h"

class MeshObject :
    public IObject
{

public: // 関数

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~MeshObject() = default;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="data">メッシュデータ</param>
    virtual void Initialize(LevelData::MeshData* data);

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void Draw(BaseCamera& camera);

    /// <summary>
    /// ImGui描画
    /// </summary>
    virtual void ImGuiDraw() override {};

    /// <summary>
    /// パーティクル描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void ParticleDraw(BaseCamera& camera){}

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager"></param>
    virtual void CollisionListRegister(BaseCollisionManager* collisionManager);

    /// <summary>
    /// コライダー登録
    /// </summary>
    /// <param name="collisionManager"></param>
    /// <param name="colliderDebugDraw"></param>
    virtual void CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw);

protected: // 関数

    /// <summary>
    /// コライダーの初期化
    /// </summary>
    /// <param name="collider">コライダー</param>
    virtual void ColliderInitialize(ColliderShape collider);

    /// <summary>
    /// 保存用速度
    /// </summary>
    virtual void SaveVelocityUpdate();

public: // アクセッサ

    ColliderShape* GetCollider() { return collider_.get(); };
    void SetCollider(ColliderShape* collider) { collider_.reset(collider); };

    /// <summary>
    /// 親の名前
    /// </summary>
    /// <returns></returns>
    std::string GetParentName() { return parentName_; }

    /// <summary>
    /// 保存用速度
    /// </summary>
    /// <returns></returns>
    Vector3 GetSaveVelocity() { return saveVelocity_; }

    void SetMaterialColor(const Vector4 color) { return material_->SetColor(color); }

protected: // 変数

    // ファイル名前
    std::string fileName_;

    // ディレクトリパス
    std::string directoryPath_;

    // モデル
    Model* model_ = nullptr;

    // マテリアル
    std::unique_ptr<Material> material_;

    // コライダー
    std::unique_ptr<ColliderShape> collider_;

    // 親の名前
    std::string parentName_;

    // 前フレームの位置
    Vector3 prePosition_;

    // 保存用速度
    Vector3 saveVelocity_;

};

