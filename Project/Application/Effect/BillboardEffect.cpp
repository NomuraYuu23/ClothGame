#include "BillboardEffect.h"
#include "../../Engine/Particle/BillBoardMatrix.h"
#include "../../Engine/Math/DeltaTime.h"
#include "../../Engine/3D/Model/ModelDraw.h"

void BillboardEffect::Initialize(Model* model, uint32_t textureHandle, float lifeTime)
{

	// モデル
	model_ = model;

	// マテリアル
	material_.reset(Material::Create());

	// ワールドトランスフォーム
	worldTransform_.Initialize(true);

	// テクスチャハンドル
	textureHnadle_ = textureHandle;

	// 初期位置
	initPosition_ = {};

	// 終了位置
	endPosition_ = {};

	// 初期大きさ
	initScale_ = { 1.0f, 1.0f, 1.0f };

	// 終了大きさ
	endScale_ = { 1.0f, 1.0f, 1.0f };

	// 初期透明度
	initAlpha_ = 1.0f;

	// 終了透明度
	endAlppha_ = 0.0f;

	// 色
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 補間係数
	t_ = 0.0f;

	// 生存時間
	lifeTime_ = lifeTime;

	// 死んでるか
	isDead_ = true;
	
	// Ease名前
	easeName_ = Ease::EaseName::Lerp;

}

void BillboardEffect::Update(BaseCamera& camera)
{

	if (isDead_) {
		return;
	}

	t_ += kDeltaTime_;
	if (t_ >= lifeTime_) {
		isDead_ = true;
	}

	// 位置
	position_ = Ease::Easing(easeName_, initPosition_, endPosition_, t_);

	// 大きさ
	scale_ = Ease::Easing(easeName_, initScale_, endScale_, t_);

	// 透明度
	alpha_ = Ease::Easing(easeName_, initAlpha_, endAlppha_, t_);
	color_.w = alpha_;
	material_->SetColor(color_);

	// ワールドトランスフォーム更新
	worldTransform_.transform_.translate = position_;
	worldTransform_.transform_.scale = scale_;
	worldTransform_.UpdateMatrix(BillBoardMatrix::GetBillBoardMatrixAll(camera));

}

void BillboardEffect::Draw(BaseCamera& camera)
{

	if (isDead_) {
		return;
	}

	ModelDraw::NormalObjectDesc desc;
	desc.camera = &camera;
	desc.material = material_.get();
	desc.model = model_;
	desc.textureHandles.push_back(textureHnadle_);
	desc.worldTransform = &worldTransform_;

	ModelDraw::NormalObjectDraw(desc);

}

void BillboardEffect::Reset()
{

	// 位置
	position_ = initPosition_;

	// 大きさ
	scale_ = initScale_;

	// 透明度
	alpha_ = initAlpha_;

	// 死んでいない
	isDead_ = false;

	// 補間係数リセット
	t_ = 0.0f;

}

