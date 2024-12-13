#include "ClothDemoCapsule.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"

void ClothDemoCapsule::Initialize(const std::string& name)
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
    material_->SetColor(Vector4{ 0.5f,1.0f,0.5f,1.0f });
    material_->SetEnableLighting(HalfLambert);

    // トランスフォーム
    worldTransform_.Initialize(true);

    // データ
    data_.origin = { 0.0f, -0.5f, 0.0f };
    data_.diff = { 0.0f, 0.5f, 0.0f };
    data_.radius = 0.5f;

    // 画面ちらつかないようの値
    screenDoesNotFlickerValue_ = 0.01f;

    // デモに存在するか
    exist_ = false;

    // 名前
    name_ = name;

    // テクスチャハンドル
    textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", dxCommon);

    // トランスフォーム、カプセルの下用
    diffWorldTransform_.Initialize(true);

    // 円柱モデルファイル名前
    cylinderFileName_ = "Cylinder.obj";

    // 円柱モデル
    cylinderModel_.reset(Model::Create(directoryPath_, cylinderFileName_, dxCommon));

    // トランスフォーム、円柱
    cylinderWorldTransform_.Initialize(true);

    // ギズモ操作番号
    guizmoOperation_ = ImGuizmo::TRANSLATE;

    // ギズモID
    guizmoID_ = nextGuizmoID_;
    nextGuizmoID_++;

}

void ClothDemoCapsule::Update()
{

    // 原点球
    // 位置
    worldTransform_.transform_.translate = data_.origin;
    // 大きさ
    float size = data_.radius - screenDoesNotFlickerValue_;
    worldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    worldTransform_.UpdateMatrix();

    // 線分球
    // 位置
    diffWorldTransform_.transform_.translate = data_.origin + data_.diff;
    // 大きさ
    diffWorldTransform_.transform_.scale = { size, size, size };

    // 行列更新
    diffWorldTransform_.UpdateMatrix();

    // 円柱
    // 位置
    cylinderWorldTransform_.transform_.translate = data_.diff * 0.5f + data_.origin;
    // 大きさ
    size = data_.radius - screenDoesNotFlickerValue_;
    cylinderWorldTransform_.transform_.scale = { size, size, Vector3::Length(data_.diff) * 0.5f };
    // 回転
    cylinderWorldTransform_.usedDirection_ = true;
    cylinderWorldTransform_.direction_ = Vector3::Normalize(data_.diff);

    // 行列更新
    cylinderWorldTransform_.UpdateMatrix();

}

void ClothDemoCapsule::Draw(BaseCamera& camera)
{

    // 出現してないなら描画しない
    if (!exist_) {
        return;
    }

    ModelDraw::NormalObjectDesc desc;
    std::vector<UINT> textureHandles;
    textureHandles.push_back(textureHandle_);

    // 原点球
    desc.model = model_.get();
    desc.material = material_.get();
    desc.camera = &camera;
    desc.worldTransform = &worldTransform_;
    desc.textureHandles = textureHandles;

    ModelDraw::NormalObjectDraw(desc);

    // 線分球
    desc.worldTransform = &diffWorldTransform_;
    ModelDraw::NormalObjectDraw(desc);

    // 円柱
    desc.model = cylinderModel_.get();
    desc.worldTransform = &cylinderWorldTransform_;
    ModelDraw::NormalObjectDraw(desc);

}

void ClothDemoCapsule::ImGuiDraw(BaseCamera& camera)
{

    ImGui::Text("カプセル");
    // 原点
    ImGui::DragFloat3("カプセル_原点", &data_.origin.x, 0.01f);
    // 終点までのベクトル
    ImGui::DragFloat3("カプセル_終点までのベクトル", &data_.diff.x, 0.01f);
    // 半径
    ImGui::DragFloat("カプセル_半径", &data_.radius, 0.01f);

    // ギズモ

    // 変数
    EulerTransform transform =
    {
        Vector3{ data_.radius, data_.radius, Vector3::Length(data_.diff) * 0.5f },
        Vector3{ 0.0f,0.0f,0.0f },
        data_.origin + (data_.diff * 0.5f)
    };
    Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(transform.scale);
    Matrix4x4 rotateMatrix = Matrix4x4::DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, Vector3::Normalize(data_.diff));
    Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform.translate);

    Matrix4x4 matrix = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));

    // モード
    std::string modeName = "カプセル位置_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::TRANSLATE)) {
        guizmoOperation_ = ImGuizmo::TRANSLATE;
    }
    ImGui::SameLine();
    modeName = "カプセル回転_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::ROTATE)) {
        guizmoOperation_ = ImGuizmo::ROTATE;
    }
    ImGui::SameLine();
    modeName = "カプセル半径_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::SCALE_X)) {
        guizmoOperation_ = ImGuizmo::SCALE_X;
    }
    ImGui::SameLine();
    modeName = "カプセル長さ_" + std::to_string(guizmoID_);
    if (ImGui::RadioButton(modeName.c_str(), guizmoOperation_ == ImGuizmo::SCALE_Z)) {
        guizmoOperation_ = ImGuizmo::SCALE_Z;
    }

    // 操作部分
    ImGuizmo::PushID(guizmoID_);
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::Manipulate(
        &camera.GetViewMatrix().m[0][0], &camera.GetProjectionMatrix().m[0][0], static_cast<ImGuizmo::OPERATION>(guizmoOperation_), ImGuizmo::LOCAL, &matrix.m[0][0], NULL, NULL);
    
    if (ImGuizmo::IsUsing()) {
        // 位置と半径を取り出す
        ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], &transform.translate.x, &transform.rotate.x, &transform.scale.x);

        // データを代入
        data_.origin = transform.translate - Matrix4x4::TransformNormal(Vector3{ 0.0f, 0.0f, 1.0f }, matrix);
        data_.diff = Matrix4x4::TransformNormal(Vector3{ 0.0f, 0.0f, 1.0f }, matrix) * 2.0f;
        data_.radius = transform.scale.x;
    }
    
    // ID
    ImGuizmo::PopID();

    // 更新
    Update();

}
