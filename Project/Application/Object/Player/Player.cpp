#include "Player.h"
#include "Collision/PlayerCollision.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../Collider/CollisionConfig.h"

// 足までの位置
const Vector3 Player::kPositionToFeet_ = { 0.0f,-1.0f,0.0f };

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(LevelData::MeshData* data)
{

	// メッシュオブジェクトの初期化
	MeshObject::Initialize(data);

	material_->SetEnableLighting(HalfLambert);

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributePlayer_;
	collisionMask_ -= kCollisionAttributePlayer_;

	// コライダー
	OBB obb = std::get<OBB>(*collider_.get());
	obb.SetParentObject(this);
	obb.SetCollisionAttribute(collisionAttribute_);
	obb.SetCollisionMask(collisionMask_);
	ColliderShape* colliderShape = new ColliderShape();
	*colliderShape = obb;
	collider_.reset(colliderShape);

	// ステートの初期化
	playerStateSystem_ = std::make_unique<PlayerStateSystem>();
	playerStateSystem_->Initialize(this);

	// アニメーションの初期化
	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->Initialize(model_);

	// 浮いているか
	floating_ = false;
	
	// ワープ
	warping_ = false;

	// レベルアップ
	levelUp_ = false;

	// 砂埃
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	runDustParticle_ = std::make_unique<RunDustParticle>();
	runDustParticle_->Initialize(
		dxCommon->GetDevice(),
		dxCommon->GetCommadListLoad(),
		GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get(),
		GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexGPUParticle].Get());

	// エミッタ設定
	const EmitterCS kEmitter =
	{
			worldTransform_.GetWorldPosition() + kPositionToFeet_, // 位置
			1.0f, // 射出半径
			10, // 射出数
			0.1f, // 射出間隔
			0.0f, // 射出間隔調整時間
			0 // 射出許可
	};
	runDustParticle_->SetEmitter(kEmitter);

}

void Player::Update()
{

	// ワープした
	if (warping_) {
		// ワープリセット
		warping_ = false;
		levelUp_ = false;
		playerStateSystem_->GetPlayerCommand()->DashReset();
	}

	// メッシュオブジェクトの更新
	MeshObject::Update();

	// ステートの更新
	playerStateSystem_->Update();

	// アニメーション更新
	playerAnimation_->Update(playerStateSystem_->GetPlayerState()->GetPlaryerMotionNo());

	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();
	
	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix();

	// 落下確認
	FallCheck();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

	// 砂埃
	const EmitterCS kEmitter =
	{
			worldTransform_.GetWorldPosition() + kPositionToFeet_, // 位置
			1.0f, // 射出半径
			2, // 射出数
			0.1f, // 射出間隔
			0.0f, // 射出間隔調整時間
			0 // 射出許可
	};
	if (!floating_) {
		runDustParticle_->SetEmitter(kEmitter, false);
	}
	else {
		runDustParticle_->SetEmitter(kEmitter, true);
	}
	runDustParticle_->Update();

	// 落下していることにする
	floating_ = true;

}

void Player::Draw(BaseCamera& camera)
{

	// アニメーションオブジェクトになるまでコメントアウト
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = playerAnimation_->GetLocalMatrixManager();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void Player::ImGuiDraw()
{

}

void Player::ParticleDraw(BaseCamera& camera)
{

	// 砂埃
	runDustParticle_->Draw(DirectXCommon::GetInstance()->GetCommadList(), camera);

}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	collisionData;

	// 地面ブロック
	if (std::holds_alternative<GroundBlock*>(colliderPartner)) {
		PlayerCollision::OnColiisionGroundBlock(this, colliderPartner);
	}
	// ゴースト
	if (std::holds_alternative<BaseEnemy*>(colliderPartner)) {
		PlayerCollision::OnColiisionEnemy(this, colliderPartner);
	}

}

void Player::Damage()
{

	// ダメージ状態へ
	playerStateSystem_->SetInterruptCommand(true);
	playerStateSystem_->SetNextStateNo(kPlayerStateIndexDamage);

}

void Player::ColliderUpdate()
{

	OBB obb = std::get<OBB>(*collider_.get());

	obb.center_ = worldTransform_.GetWorldPosition();

	obb.SetOtientatuons(worldTransform_.rotateMatrix_);

	ColliderShape* colliderShape = new ColliderShape();

	*colliderShape = obb;

	collider_.reset(colliderShape);

}

void Player::FallCheck()
{

	// 落下
	const float kFallPositionY = -10.0f;
	if (worldTransform_.GetWorldPosition().y <= kFallPositionY) {
		Damage();
	}

}
