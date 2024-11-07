#include "Skydome.h"
#include <cassert>
#include "../../Engine/2D/ImguiManager.h"
#include <numbers>
#include "../../Engine/3D/Model/ModelDraw.h"

Skydome::~Skydome()
{

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
void Skydome::Initialize(Model* model) {

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	// ワールド変換データの初期化
	worldTransform_.Initialize(model_->GetRootNode());
	worldTransform_.transform_.scale = { 2.0f,2.0f,2.0f };
	worldTransform_.UpdateMatrix();

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update() {

	worldTransform_.transform_.rotate.y = fmodf(worldTransform_.transform_.rotate.y + rotateSpeed_, static_cast<float>(std::numbers::pi) * 2.0f);

	worldTransform_.UpdateMatrix();

	localMatrixManager_->Map();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Skydome::Draw(BaseCamera& camera) {

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Skydome::ImGuiDraw()
{

	ImGui::Begin("Skydome");
	ImGui::DragFloat("rotateSpeed", &rotateSpeed_, 0.001f);
	ImGui::End();

}
