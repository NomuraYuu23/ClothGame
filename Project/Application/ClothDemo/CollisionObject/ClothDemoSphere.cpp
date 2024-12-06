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
    worldTransform_.Initialize(model_->GetRootNode());

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

    // ギズモ操作番号
    guizmoOperation_ = ImGuizmo::TRANSLATE;

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

void ClothDemoSphere::ImGuiDraw(BaseCamera& camera)
{

    ImGui::Text("sphere");
    // 法線
    ImGui::DragFloat3("sphere.position", &data_.position_.x, 0.01f);
    // 距離
    ImGui::DragFloat("sphere.radius", &data_.radius_, 0.01f, 0.01f, 1000.0f);

    // ギズモ

    // 変数
    EulerTransform transform = 
    {
        Vector3{ data_.radius_, data_.radius_, data_.radius_ },
        Vector3{ 0.0f,0.0f,0.0f },
        data_.position_
    };
    Matrix4x4 matrix = Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

    if (ImGui::RadioButton("位置", guizmoOperation_ == ImGuizmo::TRANSLATE)) {
        guizmoOperation_ = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("半径", guizmoOperation_ == ImGuizmo::SCALE_X)) {
        guizmoOperation_ = ImGuizmo::SCALE_X;
    }

    // 操作部分
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(
        &camera.GetViewMatrix().m[0][0], &camera.GetProjectionMatrix().m[0][0], static_cast<ImGuizmo::OPERATION>(guizmoOperation_), ImGuizmo::LOCAL, &matrix.m[0][0], NULL, NULL);

    // 位置と半径を取り出す
    ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], &transform.translate.x, &transform.rotate.x, &transform.scale.x);

    // データを代入
    data_.position_ = transform.translate;
    data_.radius_ = transform.scale.x;

    // 更新
    Update();

}
