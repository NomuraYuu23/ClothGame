#include "Player.h"
#include "Collision/PlayerCollision.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

#include "../Ground/Ground.h"
#include "../../Collider/CollisionConfig.h"

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

	// 衝突マスク
	collisionAttribute_ = kCollisionAttributePlayer;
	collisionMask_ -= kCollisionAttributePlayer;

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
	//playerAnimation_ = std::make_unique<PlayerAnimation>();
	//playerAnimation_->Initialize(model_);

	// 浮いているか
	floating_ = false;

}

void Player::Update()
{

	// メッシュオブジェクトの更新
	MeshObject::Update();

	// ステートの更新
	playerStateSystem_->Update();

	// アニメーション更新
	//playerAnimation_->Update(playerStateSystem_->GetPlayerState()->GetPlaryerMotionNo());

	// 重力
	worldTransform_.transform_.translate += Gravity::Execute();
	
	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Player::Draw(BaseCamera& camera)
{

	// アニメーションオブジェクトになるまでコメントアウト
	//ModelDraw::AnimObjectDesc desc;
	//desc.camera = &camera;
	//desc.localMatrixManager = playerAnimation_->GetLocalMatrixManager();
	//desc.material = material_.get();
	//desc.model = model_;
	//desc.worldTransform = &worldTransform_;
	//ModelDraw::AnimObjectDraw(desc);

	MeshObject::Draw(camera);

}

void Player::ImGuiDraw()
{

}

void Player::ParticleDraw(BaseCamera& camera)
{



}

void Player::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	// 地面
	if (std::holds_alternative<Ground*>(colliderPartner)) {
		PlayerCollision::OnColiisionGround(this, colliderPartner);
	}

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