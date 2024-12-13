#include "ClothDemoSphere.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"

void ClothDemoSphere::Initialize(const std::string& name)
{

    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // ファイル名前
    fileName_ = "ball.obj";

    // ディレクトリパス
    directoryPath_ = "Resources/default/";

    // モデル
    model_.reset(Model::Create(directoryPath_, fileName_, dxCommon));

    // マテリアル
    material_.reset(Material::Create());
    material_->SetColor(Vector4{ 1.0f,0.5f,0.5f,1.0f });
    material_->SetEnableLighting(HalfLambert);

    // トランスフォーム
    worldTransform_.Initialize(true);

    // データ
    data_.position_ = { 0.0f, 0.0f, 0.0f };
    data_.radius_ = 0.5f;

    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;

    // デモに存在するか
    exist_ = false;

    // 名前
    name_ = name;

    // テクスチャハンドル
    textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon);

}

void ClothDemoSphere::Update()
{

    // 位置
    worldTransform_.transform_.translate = data_.position_;
    float size = data_.radius_ - screenDoesNotFlickerValue_;
    worldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ClothDemoSphere::ImGuiDraw()
{

    ImGui::Text("sphere");
    // 法線
    ImGui::DragFloat3("sphere.position", &data_.position_.x, 0.01f);
    // 距離
    ImGui::DragFloat("sphere.radius", &data_.radius_, 0.01f);

}
