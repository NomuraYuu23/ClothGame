#include "ClothGate.h"
#include "../Player/Player.h"
#include "../../../Engine/Math/RandomEngine.h"

const std::string ClothGate::kPlayerColliderName_ = "player";

const Vector2 ClothGate::kClothScale_ = { 3.0f,4.0f };

const Vector2 ClothGate::kClothDiv_ = { 16.0f,16.0f };

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
	cloth_->SetMass(1.0f);
	cloth_->SetSpeedResistance(0.9f);
	cloth_->SetStiffness(100.0f); // 剛性。バネ定数k
	// 抵抗
	cloth_->SetStructuralStretch(100.0f);
	cloth_->SetStructuralShrink(100.0f);
	cloth_->SetShearStretch(70.0f);
	cloth_->SetShearShrink(70.0f);
	cloth_->SetBendingStretch(20.0f);
	cloth_->SetBendingShrink(20.0f);
	// 更新回数
	cloth_->SetRelaxation(4);

	// プレイヤーの衝突判定データ
	playerCollider_.position_ = {0.0f,0.0f,0.0f};
	const float playerColliderRadius = 2.0f;
	playerCollider_.radius_ = playerColliderRadius;
	// 登録
	cloth_->CollisionDataRegistration(kPlayerColliderName_, ClothGPUCollision::kCollisionTypeIndexSphere);

}

void ClothGate::Update()
{

	// 布CPU側処理
	ClothUpdate();

	// 球
	// プレイヤーの情報をいれる
	playerCollider_.position_ = player_->GetWorldTransformAdress()->GetWorldPosition();
	ClothGPUCollision::CollisionDataMap playerColliderData = playerCollider_;
	cloth_->CollisionDataUpdate(kPlayerColliderName_, playerColliderData);

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

	// 乱数
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 風力最大値
	const float kWindPowerMin = -10.0f;
	const float kWindPowerMax = 10.0f;
	std::uniform_real_distribution<float> distribution(kWindPowerMin, kWindPowerMax);

	// 風力
	const Vector3 wind = { distribution(randomEngine), 0.0f, distribution(randomEngine) };

	//風
	cloth_->SetWind(wind);

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
	cloth_->SetWeight(kRightX, 0, false);
	cloth_->SetPosition(kRightX, 0, kRightTopPosition);

}

void ClothGate::ImGuiDraw()
{

	cloth_->ImGuiDraw("clothGPU");

}
