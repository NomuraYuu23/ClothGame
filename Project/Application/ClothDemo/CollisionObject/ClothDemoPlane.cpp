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

   // ギズモ操作番号
   guizmoOperation_ = ImGuizmo::TRANSLATE_Z;

   // ギズモID
   guizmoID_ = nextGuizmoID_;
   nextGuizmoID_++;

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

void ClothDemoPlane::ImGuiDraw(BaseCamera& camera)
{

    ImGui::Text("平面");
    // 法線
    ImGui::DragFloat3("平面_法線", &data_.normal_.x, 0.01f);
    data_.normal_ = Vector3::Normalize(data_.normal_);
    // 距離
    ImGui::DragFloat("平面_距離", &data_.distance_, 0.01f);

    // ギズモ

    // 変数
    EulerTransform transform =
    {
        kScale_,
        Vector3{ 0.0f,0.0f,0.0f },
        Vector3{ 0.0f, data_.distance_, 0.0f },
    };

    Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(kScale_);
    Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, data_.normal_);
    Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(Vector3{ 0.0f, data_.distance_, 0.0f });

    Matrix4x4 matrix = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));

    // モード
    std::string modeName = "平面距離_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::TRANSLATE_Z)) {
        guizmoOperation_ = ImGuizmo::TRANSLATE_Z;
    }
    ImGui::SameLine();
    modeName = "平面法線_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::ROTATE)) {
        guizmoOperation_ = ImGuizmo::ROTATE;
    }

    // 操作部分
    // ID
    ImGuizmo::PushID(guizmoID_);
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(
        &camera.GetViewMatrix().m[0][0], &camera.GetProjectionMatrix().m[0][0], static_cast<ImGuizmo::OPERATION>(guizmoOperation_), ImGuizmo::LOCAL, &matrix.m[0][0], NULL, NULL);
    // ID
    ImGuizmo::PopID();

    // 位置と半径を取り出す
    ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], &transform.translate.x, &transform.rotate.x, &transform.scale.x);

    // データを代入
    data_.distance_ = transform.translate.y;
    data_.normal_ = Matrix4x4::TransformNormal(Vector3{ 0.0f,0.0f,1.0f },matrix);
    data_.normal_ = Vector3::Normalize(data_.normal_);

    // 更新
    Update();

}
