#pragma once
#include "ClothDemoObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

/// <summary>
/// クロスシミュレーションデモのカプセルオブジェクト
/// </summary>
class ClothDemoCapsule :
    public ClothDemoObject
{

public:

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="name">名前</param>
    void Initialize(const std::string& name) override;

    /// <summary>
    /// 更新
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
    /// <param name="camera"></param>
    void ImGuiDraw(BaseCamera& camera) override;

    /// <summary>
    /// データ取得
    /// </summary>
    /// <returns></returns>
    ClothGPUCollision::Capsule GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Capsule data_;

    // トランスフォーム、カプセルの下用
    WorldTransform diffWorldTransform_;

    // 円柱モデル
    std::unique_ptr<Model> cylinderModel_;

    // 円柱モデルファイル名前
    std::string cylinderFileName_;

    // トランスフォーム、円柱用
    WorldTransform cylinderWorldTransform_;

};

