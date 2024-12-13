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
    const Vector4 kMaterialColor = { 1.0f,0.5f,0.5f,1.0f };
    material_.reset(Material::Create());
    material_->SetColor(kMaterialColor);
    material_->SetEnableLighting(HalfLambert);

    // トランスフォーム
    worldTransform_.Initialize(true);

    // データ
    const ClothGPUCollision::Sphere kInitData =
    {
        Vector3{ 0.0f, 0.0f, 0.0f },
        0.5f
    };
    data_ = kInitData;

    // 画面ちらつかないようの値
    const float kInitScreenDoesNotFlickerValue = 0.01f;
    screenDoesNotFlickerValue_ = kInitScreenDoesNotFlickerValue;

    // デモに存在するか
    exist_ = false;

    // 名前
    name_ = name;

    // テクスチャハンドル
    textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon);

    // ギズモ操作番号
    guizmoOperation_ = ImGuizmo::TRANSLATE;

    // ギズモID
    guizmoID_ = nextGuizmoID_;
    nextGuizmoID_++;

}

void ClothDemoSphere::Update()
{

    // 位置
    worldTransform_.transform_.translate = data_.position;
    float size = data_.radius - screenDoesNotFlickerValue_;
    worldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    worldTransform_.UpdateMatrix();

}

void ClothDemoSphere::ImGuiDraw(BaseCamera& camera)
{

    // ImGui速度
    const float kImGuiSpeed = 0.01f;

    ImGui::Text("球");
    // 法線
    ImGui::DragFloat3("球_位置", &data_.position.x, kImGuiSpeed);
    // 距離
    ImGui::DragFloat("球_半径", &data_.radius, kImGuiSpeed);

    // ギズモ

    // 変数
    EulerTransform transform = 
    {
        Vector3{ data_.radius, data_.radius, data_.radius },
        Vector3{ 0.0f,0.0f,0.0f },
        data_.position
    };
    Matrix4x4 matrix = Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

    // モード
    std::string modeName = "球位置_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::TRANSLATE)) {
        guizmoOperation_ = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    modeName = "球半径_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::SCALE_X)) {
        guizmoOperation_ = ImGuizmo::SCALE_X;
    }

    // 操作部分
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
    data_.position = transform.translate;
    data_.radius = transform.scale.x;

    // 更新
    Update();

}
