#include "Ghost.h"
#include "../../../Engine/Physics/Gravity/Gravity.h"

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

	// アニメーションの初期化
	//ghostAnimation_ = std::make_unique<GhostAnimation>();
	//ghostAnimation_->Initialize(model_);

}

void Ghost::Update()
{


	// メッシュオブジェクトの更新
	MeshObject::Update();

	// ステートの更新
	ghostStateSystem_->Update();

	// アニメーション更新
	//ghostAnimation_->Update(playerStateSystem_->GetPlayerState()->GetPlaryerMotionNo());

	// ワールドトランスフォーム更新
	worldTransform_.UpdateMatrix();

	// コライダー
	ColliderUpdate();

	// 速度保存
	SaveVelocityUpdate();


}

void Ghost::Draw(BaseCamera& camera)
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

void Ghost::ImGuiDraw()
{
}

void Ghost::ParticleDraw(BaseCamera& camera)
{
}

void Ghost::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{
}
