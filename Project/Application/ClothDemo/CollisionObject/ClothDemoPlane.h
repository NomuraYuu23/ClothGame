#pragma once
#include "ClothDemoObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPUCollision.h"

/// <summary>
/// クロスシミュレーションデモの平面オブジェクト
/// </summary>
class ClothDemoPlane : 
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
    ClothGPUCollision::Plane GetData() { return data_; }

private:

    // ClothGPUに渡すデータ
    ClothGPUCollision::Plane data_;

    // 画面ちらつかないようの値
    float screenDoesNotFlickerValue_;

    // 大きさ
    const Vector3 kScale_ = { 10.0f,10.0f,10.0f };

};
