#include "ClothDemoPlane.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"

void ClothDemoPlane::Initialize(const std::string& name)
{

    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    // ファイル名前
    fileName_ = "plane.obj";

    // ディレクトリパス
    directoryPath_ = "Resources/default/";

    // モデル
    model_.reset(Model::Create(directoryPath_, fileName_, dxCommon));

    // マテリアル
    material_.reset(Material::Create());
    material_->SetColor(Vector4{0.5f,0.5f,1.0f,1.0f});
    material_->SetEnableLighting(HalfLambert);

    // トランスフォーム
    worldTransform_.Initialize(model_->GetRootNode());

    // データ
    data_.distance_ = 0.0f;
    data_.normal_ = {0.0f, 1.0f, 0.0f};
    
    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;
    
    // 大きさ
    worldTransform_.transform_.scale = kScale_;
    
    // デモに存在するか
    exist_ = false;

    // 名前
   name_ = name;

   // テクスチャハンドル
   textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon);

}

void ClothDemoPlane::Update()
{

    // 回転
    worldTransform_.usedDirection_ = true;
    worldTransform_.direction_ = data_.normal_;
    
    // 位置
    worldTransform_.transform_.translate = data_.normal_ * (data_.distance_ - screenDoesNotFlickerValue_);

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ClothDemoPlane::ImGuiDraw()
{

    ImGui::Text("plane");
    // 法線
    ImGui::DragFloat3("plane.normal", &data_.normal_.x, 0.01f);
    data_.normal_ = Vector3::Normalize(data_.normal_);
    // 距離
    ImGui::DragFloat("plane.distance", &data_.distance_, 0.01f);

}
