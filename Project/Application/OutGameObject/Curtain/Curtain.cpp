#include "Curtain.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/RandomEngine.h"
#include "../../../Engine/Math/DeltaTime.h"

const Vector2 Curtain::kClothScale_ = { 10.0f, 5.0f };

const Vector2 Curtain::kClothDiv_ = { 16.0f,16.0f };

const Vector3 Curtain::kBaseFixed_ = { 8.0f, 3.0f, 0.0f };

const Vector3 Curtain::kBaseMovingOpen_{ 7.0f, 3.0f, 0.0f };

const Vector3 Curtain::kBaseMovingClose_{ 0.0f, 3.0f, 0.0f };

DirectXCommon* Curtain::dxCommon_ = DirectXCommon::GetInstance();

void Curtain::Initialize()
{

	// 右布
	rightCurtain_ = std::make_unique<ClothGPU>();
	// 初期化
	rightCurtain_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale_, kClothDiv_, "Resources/Model/SideCloth/Cloth.png");
	// マテリアル
	rightCurtain_->GetMaterial()->SetEnableLighting(HalfLambert);

	// 左布
	leftCurtain_ = std::make_unique<ClothGPU>();
	// 初期化
	leftCurtain_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale_, kClothDiv_, "Resources/Model/SideCloth/Cloth.png");
	// マテリアル
	leftCurtain_->GetMaterial()->SetEnableLighting(HalfLambert);

	// 布の計算データ
	// 質量
	const float kClothMass = 7.0f;
	rightCurtain_->SetMass(kClothMass);
	leftCurtain_->SetMass(kClothMass);
	// 速度抵抗
	const float kClothSpeedResistance = 0.0f;
	rightCurtain_->SetSpeedResistance(kClothSpeedResistance);
	leftCurtain_->SetSpeedResistance(kClothSpeedResistance);
	// 剛性。バネ定数k
	const float kClothStiffness = 100.0f;
	rightCurtain_->SetStiffness(kClothStiffness);
	leftCurtain_->SetStiffness(kClothStiffness);
	// 抵抗
	const float kClothStructural = 100.0f;
	rightCurtain_->SetStructuralStretch(kClothStructural);
	rightCurtain_->SetStructuralShrink(kClothStructural);
	leftCurtain_->SetStructuralStretch(kClothStructural);
	leftCurtain_->SetStructuralShrink(kClothStructural);
	const float kClothShear = 70.0f;
	rightCurtain_->SetShearStretch(kClothShear);
	rightCurtain_->SetShearShrink(kClothShear);
	leftCurtain_->SetShearStretch(kClothShear);
	leftCurtain_->SetShearShrink(kClothShear);
	const float kClothBending = 20.0f;
	rightCurtain_->SetBendingStretch(kClothBending);
	rightCurtain_->SetBendingShrink(kClothBending);
	leftCurtain_->SetBendingStretch(kClothBending);
	leftCurtain_->SetBendingShrink(kClothBending);
	// 速度制限
	const float kClothVelocityLimit = 0.09f;
	rightCurtain_->SetVelocityLimit(kClothVelocityLimit);
	leftCurtain_->SetVelocityLimit(kClothVelocityLimit);
	// 更新回数
	const uint32_t kClothRelaxation = 6;
	rightCurtain_->SetRelaxation(kClothRelaxation);
	leftCurtain_->SetRelaxation(kClothRelaxation);

	// 位置
	rightMovingPosition_ = kBaseMovingOpen_;
	leftMovingPosition_ = kBaseMovingOpen_;

}

void Curtain::Update(float t)
{

	// 乱数
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 風力最大値
	const float kWindPowerMin = -1.0f;
	const float kWindPowerMax = 1.0f;
	std::uniform_real_distribution<float> distribution(kWindPowerMin, kWindPowerMax);

	// 風力
	const Vector3 wind = { distribution(randomEngine) * 10.0f, 0.0f, distribution(randomEngine) * 10.0f };

	//風
	rightCurtain_->SetWind(wind);
	leftCurtain_->SetWind(wind);

	// 布更新
	rightCurtain_->Update();
	leftCurtain_->Update();

	// 固定部分

	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			// 重み
			rightCurtain_->SetWeight(y, x, true);
			leftCurtain_->SetWeight(y, x, true);
		}
	}


	rightMovingPosition_ = Ease::Easing(Ease::EaseName::Lerp, kBaseMovingOpen_, kBaseMovingClose_, t);
	const Vector3 kBaseMovingOpenLeft = { -kBaseMovingOpen_.x, kBaseMovingOpen_.y, kBaseMovingOpen_.z };
	leftMovingPosition_ = Ease::Easing(Ease::EaseName::Lerp, kBaseMovingOpenLeft, kBaseMovingClose_, t);

	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	const uint32_t step = 4;
	for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); x += step) {

		// 右

		// 重み
		rightCurtain_->SetWeight(0, x, true);
		// 位置
		resetPosition = kBaseFixed_;
		resetPosition.x = Ease::Easing(Ease::EaseName::Lerp, rightMovingPosition_.x, kBaseFixed_.x, static_cast<float>(x) / kClothDiv_.x);
		rightCurtain_->SetPosition(0, x, resetPosition);

		// 左

		// 重み
		leftCurtain_->SetWeight(0, x, true);
		// 位置
		resetPosition = kBaseFixed_;
		resetPosition.x = Ease::Easing(Ease::EaseName::Lerp, -kBaseFixed_.x, leftMovingPosition_.x, static_cast<float>(x) / kClothDiv_.x);
		leftCurtain_->SetPosition(0, x, resetPosition);

	}

}

void Curtain::Draw(BaseCamera& camera)
{

	rightCurtain_->Draw(dxCommon_->GetCommadList(), &camera);
	leftCurtain_->Draw(dxCommon_->GetCommadList(), &camera);

}
