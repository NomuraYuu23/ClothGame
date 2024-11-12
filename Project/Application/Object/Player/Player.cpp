#include "Player.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

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

	// ステートの初期化
	playerStateSystem_ = std::make_unique<PlayerStateSystem>();
	playerStateSystem_->Initialize(this);

	// アニメーションの初期化
	playerAnimation_ = std::make_unique<PlayerAnimation>();
	playerAnimation_->Initialize(model_);

}

void Player::Update()
{

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

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();

}

void Player::Draw(BaseCamera& camera)
{

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
