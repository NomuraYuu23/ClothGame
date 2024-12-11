#pragma once
#include "ClothDemoObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

/// <summary>
/// クロスシミュレーションデモの球オブジェクト
/// </summary>
class ClothDemoSphere :
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
    /// ImGui描画
    /// </summary>
    void ImGuiDraw() override;

    /// <summary>
    /// データ取得
    /// </summary>
    /// <returns></returns>
    ClothGPUCollision::Sphere GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Sphere data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

};

