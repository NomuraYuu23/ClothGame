#include "Ghost.h"
#include "../../../Engine/Math/RandomEngine.h"
#include "../../../Engine/Math/Ease.h"

#include "../../Player/Player.h"

// 自分の衝突判定データの名前
const std::string Ghost::kMyColliderName_ = "ghost";
// 大きさ
const Vector2 Ghost::kClothScale_ = { 3.0f, 3.0f };;
// 分割数
const Vector2 Ghost::kClothDiv_ = { 16.0f,16.0f };
// ワールド座標からの固定部分上
const Vector3 Ghost::kBaseFixedTop_ = { 0.0f,1.7f,0.0f };
// ワールド座標からの固定部分左
const Vector3 Ghost::kBaseFixedLeft_ = { -0.7f,1.0f,0.0f };
// ワールド座標からの固定部分右
const Vector3 Ghost::kBaseFixedRight_ = { 0.7f,1.0f,0.0f };
// ワールド座標からの固定部分前
const Vector3 Ghost::kBaseFixedFront_ = { 0.0f,1.0f,-0.7f };
// ワールド座標からの固定部分後ろ
const Vector3 Ghost::kBaseFixedBack_ = { 0.0f,1.0f,0.7f };
// DirectX
DirectXCommon* Ghost::dxCommon_ = DirectXCommon::GetInstance();

Ghost::Ghost()
{
}

Ghost::~Ghost()
{
}

void Ghost::Initialize(LevelData::MeshData* data)
{

	// エネミーの共通初期化
	BaseEnemy::Initialize(data);

	// ステートの初期化
	ghostStateSystem_ = std::make_unique<GhostStateSystem>();
	ghostStateSystem_->Initialize(this);

	// 布
	ClothInitialize();

}

void Ghost::Update()
{

	// メッシュオブジェクトの更新
	MeshObject::Update();

	// ステートの更新
	ghostStateSystem_->Update();

	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix();

	// 布
	ClothUpdate();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Ghost::Draw(BaseCamera& camera)
{

	MeshObject::Draw(camera);

	// 布
	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void Ghost::ImGuiDraw()
{

}

void Ghost::ParticleDraw(BaseCamera& camera)
{
	camera;
}

void Ghost::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	collisionData;

	// プレイヤー
	if (std::holds_alternative<Player*>(colliderPartner) && conflictWithPlayer_) {
		
		Player* player = std::get<Player*>(colliderPartner);
		
		if (player->GetCurrentStateNo() == kPlayerStateIndexDash) {
			ghostStateSystem_->SetNextStateNo(kGhostStateIndexBlownAway);
			ghostStateSystem_->SetInterruptCommand(true);
			conflictWithPlayer_ = false;
		}

	}

}

void Ghost::ClothReset()
{

	// 位置をリセット
	Vector3 resetPosition = { 0.0f,0.0f,0.0f };
	for (uint32_t y = 0; y <= static_cast<uint32_t>(kClothDiv_.y); ++y) {
		for (uint32_t x = 0; x <= static_cast<uint32_t>(kClothDiv_.x); ++x) {
			// 重み
			cloth_->SetWeight(y, x, true);
			// 位置
			resetPosition = worldTransform_.GetWorldPosition() + kBaseFixedTop_;
			resetPosition.x += Ease::Easing(Ease::EaseName::Lerp, -kClothScale_.x, kClothScale_.x, static_cast<float>(x) / kClothDiv_.x);
			resetPosition.z += Ease::Easing(Ease::EaseName::Lerp, -kClothScale_.y, kClothScale_.y, static_cast<float>(y) / kClothDiv_.y);
			cloth_->SetPosition(y, x, resetPosition);
		}
	}

}

void Ghost::ClothInitialize()
{

	// 布
	cloth_ = std::make_unique<ClothGPU>();
	// 初期化
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale_, kClothDiv_, "Resources/Model/Enemy/Ghost/GhostCloth.png");

	// 布の計算データ
	// 質量
	const float kClothMass = 0.5f;
	cloth_->SetMass(kClothMass);
	// 速度抵抗
	const float kClothSpeedResistance = 0.02f;
	cloth_->SetSpeedResistance(kClothSpeedResistance);
	// 剛性。バネ定数k
	const float kClothStiffness = 25.0f;
	cloth_->SetStiffness(kClothStiffness);
	// 抵抗
	const float kClothStructural = 10.0f;
	cloth_->SetStructuralStretch(kClothStructural);
	cloth_->SetStructuralShrink(kClothStructural);
	const float kClothShear = 1.0f;
	cloth_->SetShearStretch(kClothShear);
	cloth_->SetShearShrink(kClothShear);
	const float kClothBending = 1.0f;
	cloth_->SetBendingStretch(kClothBending);
	cloth_->SetBendingShrink(kClothBending);
	// 速度制限
	const float kClothVelocityLimit = 1000.0f;
	cloth_->SetVelocityLimit(kClothVelocityLimit);
	// 更新回数
	const uint32_t kClothRelaxation = 6;
	cloth_->SetRelaxation(kClothRelaxation);

	// プレイヤーの衝突判定データ
	const float playerColliderRadius = 0.6f;
	collider_.radius = playerColliderRadius;
	collider_.position = worldTransform_.GetWorldPosition();
	collider_.position.y += kBaseFixedTop_.y - playerColliderRadius;
	// 登録
	cloth_->CollisionDataRegistration(kMyColliderName_, ClothGPUCollision::kCollisionTypeIndexSphere);

}

void Ghost::ClothUpdate()
{

	// 乱数
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 風力最大値
	const float kWindPowerMin = -2.0f;
	const float kWindPowerMax = 2.0f;
	std::uniform_real_distribution<float> distribution(kWindPowerMin, kWindPowerMax);

	// 風力
	const Vector3 wind = { distribution(randomEngine) * 10.0f, 0.0f, distribution(randomEngine) * 10.0f };

	//風
	cloth_->SetWind(wind);

	// 布更新
	cloth_->Update();

	// 固定部分

	// 右質点位置
	const uint32_t kFixedX = static_cast<uint32_t>(kClothDiv_.x / 2.0f);
	const uint32_t kFixedY = static_cast<uint32_t>(kClothDiv_.y / 2.0f);

	// 座標
	const Vector3 kFixedPosition = worldTransform_.GetWorldPosition() + kBaseFixedTop_;
	// 固定点距離
	const uint32_t kFixedPointDistance = 3;

	// 設定
	cloth_->SetWeight(kFixedY, kFixedX, false);
	cloth_->SetPosition(kFixedY, kFixedX, kFixedPosition);

	cloth_->SetWeight(kFixedY, kFixedX + kFixedPointDistance, false);
	cloth_->SetPosition(kFixedY, kFixedX + kFixedPointDistance, worldTransform_.GetWorldPosition() + kBaseFixedRight_);

	cloth_->SetWeight(kFixedY, kFixedX - kFixedPointDistance, false);
	cloth_->SetPosition(kFixedY, kFixedX - kFixedPointDistance, worldTransform_.GetWorldPosition() + kBaseFixedLeft_);

	cloth_->SetWeight(kFixedY + kFixedPointDistance, kFixedX, false);
	cloth_->SetPosition(kFixedY + kFixedPointDistance, kFixedX, worldTransform_.GetWorldPosition() + kBaseFixedBack_);

	cloth_->SetWeight(kFixedY - kFixedPointDistance, kFixedX, false);
	cloth_->SetPosition(kFixedY - kFixedPointDistance, kFixedX, worldTransform_.GetWorldPosition() + kBaseFixedFront_);

	// 球
	// 情報をいれる
	collider_.position = worldTransform_.GetWorldPosition();
	collider_.position.y += kBaseFixedTop_.y - collider_.radius;
	ClothGPUCollision::CollisionDataMap colliderData = collider_;
	cloth_->CollisionDataUpdate(kMyColliderName_, colliderData);

}
