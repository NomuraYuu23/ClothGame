#include "SideCloth.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/RandomEngine.h"

const Vector2 SideCloth::kClothScale_ = { 50.0f,20.0f };

const Vector2 SideCloth::kClothDiv_ = { 32.0f,16.0f };

const Vector3 SideCloth::kBaseLeftFixed_ = { 0.0f, 20.0f, -2.0f };

const Vector3 SideCloth::kBaseRightFixed_ = { 0.0f, 20.0f, -48.0f };

DirectXCommon* SideCloth::dxCommon_ = DirectXCommon::GetInstance();

void SideCloth::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

	// 布
	ClothInitialize();

}

void SideCloth::Update()
{

	// 布CPU側処理
	ClothUpdate();

}

void SideCloth::Draw(BaseCamera& camera)
{

	// 本体
	MeshObject::Draw(camera);

	// 布
	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void SideCloth::ImGuiDraw()
{

}

void SideCloth::ClothReset()
{

	// 位置をリセット
	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			// 重み
			cloth_->SetWeight(y, x, true);
			// 位置
			resetPosition = worldTransform_.GetWorldPosition();
			resetPosition.z += Ease::Easing(Ease::EaseName::Lerp, kBaseLeftFixed_.z, kBaseRightFixed_.z, static_cast<float>(x) / kClothDiv_.x);
			resetPosition.y += Ease::Easing(Ease::EaseName::Lerp, kBaseRightFixed_.y, 0.0f, static_cast<float>(y) / kClothDiv_.y);
			cloth_->SetPosition(y, x, resetPosition);
		}
	}

}

void SideCloth::ClothInitialize()
{

	// 布
	cloth_ = std::make_unique<ClothGPU>();
	// 初期化
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale_, kClothDiv_, "Resources/Model/SideCloth/Cloth.png");
	// マテリアル
	cloth_->GetMaterial()->SetEnableLighting(BlinnPhongReflection);

	// 布の計算データ
	// 質量
	const float kClothMass = 7.0f;
	cloth_->SetMass(kClothMass);
	// 速度抵抗
	const float kClothSpeedResistance = 0.0f;
	cloth_->SetSpeedResistance(kClothSpeedResistance);
	// 剛性。バネ定数k
	const float kClothStiffness = 100.0f;
	cloth_->SetStiffness(kClothStiffness);
	// 抵抗
	const float kClothStructural = 100.0f;
	cloth_->SetStructuralStretch(kClothStructural);
	cloth_->SetStructuralShrink(kClothStructural);
	const float kClothShear = 70.0f;
	cloth_->SetShearStretch(kClothShear);
	cloth_->SetShearShrink(kClothShear);
	const float kClothBending = 20.0f;
	cloth_->SetBendingStretch(kClothBending);
	cloth_->SetBendingShrink(kClothBending);
	// 速度制限
	const float kClothVelocityLimit = 0.09f;
	cloth_->SetVelocityLimit(kClothVelocityLimit);
	// 更新回数
	const uint32_t kClothRelaxation = 6;
	cloth_->SetRelaxation(kClothRelaxation);

}

void SideCloth::ClothUpdate()
{

	// 乱数
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 風力最大値
	const float kWindPowerMin = -5.0f;
	const float kWindPowerMax = 5.0f;
	std::uniform_real_distribution<float> distribution(kWindPowerMin, kWindPowerMax);

	// 風力
	const Vector3 wind = { distribution(randomEngine) * 10.0f, 0.0f, distribution(randomEngine) * 10.0f };

	//風
	cloth_->SetWind(wind);

	// 布更新
	cloth_->Update(dxCommon_->GetCommadList());

	// 固定部分

	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	const uint32_t step = 4;
	for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); x += step) {
		// 重み
		cloth_->SetWeight(0, x, true);
		// 位置
		resetPosition = worldTransform_.GetWorldPosition();
		resetPosition.z += Ease::Easing(Ease::EaseName::Lerp, kBaseLeftFixed_.z, kBaseRightFixed_.z, static_cast<float>(x) / kClothDiv_.x);
		resetPosition.y += kBaseRightFixed_.y;
		cloth_->SetPosition(0, x, resetPosition);
	}

}
