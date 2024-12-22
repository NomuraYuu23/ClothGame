#include "BaseClothGate.h"
#include "../Player/Player.h"
#include "../../../Engine/Math/RandomEngine.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../Collider/CollisionConfig.h"

const std::string BaseClothGate::kPlayerColliderName_ = "player";

DirectXCommon* BaseClothGate::dxCommon_ = DirectXCommon::GetInstance();

void BaseClothGate::Initialize(LevelData::MeshData* data)
{

	// メッシュオブジェクト
	MeshObject::Initialize(data);
	// マテリアル
	material_->SetEnableLighting(BlinnPhongReflection);

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributeClothGate_;
	// プレイヤー以外と当たらない
	collisionMask_ -= kCollisionAttributeClothGate_;
	collisionMask_ -= kCollisionAttributeEnemy_;
	collisionMask_ -= kCollisionAttributeGround_;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

}

void BaseClothGate::Update()
{

	// 布CPU側処理
	ClothUpdate();

}

void BaseClothGate::Draw(BaseCamera& camera)
{

	// 本体
	MeshObject::Draw(camera);

	// 布
	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}

void BaseClothGate::ClothInitialize(const Vector2& clothScale, const Vector2& clothDiv)
{

	// 布
	cloth_ = std::make_unique<ClothGPU>();
	// 初期化
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), clothScale, clothDiv, "Resources/Model/ClothGate/Cloth.dds");

	// 布の計算データ
	// 質量
	const float kClothMass = 1.0f;
	cloth_->SetMass(kClothMass);
	// 速度抵抗
	const float kClothSpeedResistance = 0.02f;
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
	const float kClothVelocityLimit = 10.00f;
	cloth_->SetVelocityLimit(kClothVelocityLimit);
	// 更新回数
	const uint32_t kClothRelaxation = 6;
	cloth_->SetRelaxation(kClothRelaxation);

	// プレイヤーの衝突判定データ
	playerCollider_.origin = { 0.0f,0.0f,0.0f };
	playerCollider_.diff = { 0.0f,0.0f, 0.0f };
	const float playerColliderRadius = 2.0f;
	playerCollider_.radius = playerColliderRadius;
	// 登録
	cloth_->CollisionDataRegistration(kPlayerColliderName_, ClothGPUCollision::kCollisionTypeIndexCapsule);

	// 更新秒数
	const float kInitUpdateSeconds = 2.8f;
	updateSeconds_ = kInitUpdateSeconds;

}

void BaseClothGate::ClothUpdate()
{

	// 球
	// プレイヤーの情報をいれる
	const Vector3 prePlayerPosition = playerCollider_.origin;
	playerCollider_.origin = player_->GetWorldTransformAdress()->GetWorldPosition();
	playerCollider_.diff = Vector3::Subtract(prePlayerPosition,playerCollider_.origin);

	// プレイヤー近いなら更新
	const float kCollisionDistance = 10.0f; // 距離判定
	if (Vector3::Length(playerCollider_.origin - worldTransform_.GetWorldPosition()) < kCollisionDistance) {
		// 登録済み
		if (registeringPlayer_) {
			ClothGPUCollision::CollisionDataMap playerColliderData = playerCollider_;
			cloth_->CollisionDataUpdate(kPlayerColliderName_, playerColliderData);
		}
		// 登録してない
		else {
			cloth_->CollisionDataRegistration(kPlayerColliderName_, ClothGPUCollision::kCollisionTypeIndexCapsule);
			ClothGPUCollision::CollisionDataMap playerColliderData = playerCollider_;
			cloth_->CollisionDataUpdate(kPlayerColliderName_, playerColliderData);
			registeringPlayer_ = true;
		}
	}
	// プレイヤー遠い時の処理
	else {
		// 登録解除
		if (registeringPlayer_) {
			cloth_->CollisionDataDelete(kPlayerColliderName_);
			registeringPlayer_ = false;
			updateSeconds_ = 0.0f;
		}
		// 3秒猶予を持たせる
		const float kUpdateEndSeconds = 3.0f;
		// 更新フレーム
		updateSeconds_ += kDeltaTime_;
		if (updateSeconds_ >= kUpdateEndSeconds) {
			updateSeconds_ = kUpdateEndSeconds;
			return;
		}

	}

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
	cloth_->Update();

}
