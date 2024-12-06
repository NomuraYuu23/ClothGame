#pragma once
#include "../../../Engine/3D/Transform/WorldTransform.h"
#include "../../../Engine/3D/Model/Model.h"

class ClothDemoObject
{

protected: // 静的メンバ変数

    // 次のギズモID
    static uint32_t nextGuizmoID_;

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="name">名前</param>
    virtual void Initialize(const std::string& name) = 0;

    /// <summary>
    /// 更新
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void Draw(BaseCamera& camera);
    
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void ImGuiDraw(BaseCamera& camera) = 0;

public: // アクセッサ

    /// <summary>
    /// デモに存在するか
    /// </summary>
    /// <returns></returns>
    bool GetExist() { return exist_; }

    /// <summary>
    /// デモに存在するか
    /// </summary>
    /// <param name="exist"></param>
    void SetExsit(bool exist) { exist_ = exist; }

    /// <summary>
    /// 名前
    /// </summary>
    /// <returns></returns>
    std::string GetName() { return name_; }

    /// <summary>
    /// ギズモID取得
    /// </summary>
    /// <param name="guizmoID">ギズモID</param>
    void SetGuizmoID(uint32_t guizmoID) { guizmoID_ = guizmoID; }

protected:

    // トランスフォーム
    WorldTransform worldTransform_;

    // ファイル名前
    std::string fileName_;

    // ディレクトリパス
    std::string directoryPath_;

    // モデル
    std::unique_ptr<Model> model_ = nullptr;

    // マテリアル
    std::unique_ptr<Material> material_;

    // デモに存在するか
    bool exist_;

    // 名前
    std::string name_;

    // テクスチャハンドル
    uint32_t textureHandle_;

    // ギズモ操作番号
    uint32_t guizmoOperation_;

    // ギズモID
    uint32_t guizmoID_;

};

