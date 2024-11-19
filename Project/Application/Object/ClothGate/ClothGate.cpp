#include "ClothGate.h"
#include "../Player/Player.h"
#include "../../../Engine/Math/RandomEngine.h"

const std::string ClothGate::kPlayerColliderName_ = "player";

const Vector2 ClothGate::kClothScale_ = { 4.0f,3.0f };

const Vector2 ClothGate::kClothDiv_ = { 8.0f,8.0f };

DirectXCommon* ClothGate::dxCommon_ = DirectXCommon::GetInstance();

void ClothGate::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

	// 布
	cloth_ = std::make_unique<ClothGPU>();
	// 初期化
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale_, kClothDiv_, "Resources/Sprite/Cloth/BlueCloth.png");

	// 位置をリセット
	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			cloth_->SetWeight(y, x, true);
			cloth_->SetPosition(y, x, worldTransform_.GetWorldPosition());
		}
	}

	// 布の計算データ
	// 質量
	const float kClothMass = 1.0f;
	cloth_->SetMass(kClothMass);
	// 速度抵抗
	const float kClothSpeedResistance = 0.9f;
	cloth_->SetSpeedResistance(kClothSpeedResistance);
	// 剛性。バネ定数k
	const float kClothStiffness = 100.0f;
	cloth_->SetStiffness(kClothStiffness);
	// 抵抗
	const float kClothStructural = 100.0f;
	cloth_->SetStructuralStretch(100.0f);
	cloth_->SetStructuralShrink(100.0f);
	const float kClothShear = 70.0f;
	cloth_->SetShearStretch(kClothShear);
	cloth_->SetShearShrink(kClothShear);
	const float kClothBending = 20.0f;
	cloth_->SetBendingStretch(kClothBending);
	cloth_->SetBendingShrink(kClothBending);
	// 速度制限
	const float kClothVelocityLimit = 0.02f;
	cloth_->SetVelocityLimit(kClothVelocityLimit);
	// 更新回数
	const uint32_t kClothRelaxation = 6;
	cloth_->SetRelaxation(kClothRelaxation);

	// プレイヤーの衝突判定データ
	playerCollider_.origin_ = {0.0f,0.0f,0.0f};
	playerCollider_.diff_ = { 0.0f,0.0f, 1.0f };
	const float playerColliderRadius = 2.0f;
	playerCollider_.radius_ = playerColliderRadius;
	// 登録
	cloth_->CollisionDataRegistration(kPlayerColliderName_, ClothGPUCollision::kCollisionTypeIndexCapsule);

}

void ClothGate::Update()
{

	// 布CPU側処理
	ClothUpdate();

}

void ClothGate::Draw(BaseCamera& camera)
{

	// 本体
	MeshObject::Draw(camera);

	// 布
	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void ClothGate::ClothUpdate()
{

	//// 乱数
	//std::random_device seedGenerator;
	//std::mt19937 randomEngine(seedGenerator());

	//// 風力最大値
	//const float kWindPowerMin = -5.0f;
	//const float kWindPowerMax = 5.0f;
	//std::uniform_real_distribution<float> distribution(kWindPowerMin, kWindPowerMax);

	//// 風力
	//const Vector3 wind = { distribution(randomEngine) * 10.0f, 0.0f, distribution(randomEngine) * 10.0f };

	////風
	//cloth_->SetWind(wind);

	// 布更新
	cloth_->Update(dxCommon_->GetCommadList());

	// 固定部分
	
	// 右質点位置
	const uint32_t kRightX = static_cast<uint32_t>(kClothDiv_.x);

	// ワールド座標からの移動距離
	const Vector3 kBaseLeftMove = { -2.0f, 3.0f, 0.0f };
	const Vector3 kBaseRightMove = { 2.0f, 3.0f, 0.0f };

	// 座標
	const Vector3 kLeftTopPosition = worldTransform_.GetWorldPosition() + kBaseLeftMove;
	const Vector3 kRightTopPosition = worldTransform_.GetWorldPosition() + kBaseRightMove;

	// 設定
	cloth_->SetWeight(0, 0, false);
	cloth_->SetPosition(0, 0, kLeftTopPosition);
	cloth_->SetWeight(0, kRightX, false);
	cloth_->SetPosition(0, kRightX, kRightTopPosition);

	// 球
	// プレイヤーの情報をいれる
	playerCollider_.origin_ = player_->GetWorldTransformAdress()->GetWorldPosition();
	ClothGPUCollision::CollisionDataMap playerColliderData = playerCollider_;
	cloth_->CollisionDataUpdate(kPlayerColliderName_, playerColliderData);

}

void ClothGate::ImGuiDraw()
{

	cloth_->ImGuiDraw("clothGPU");

}
