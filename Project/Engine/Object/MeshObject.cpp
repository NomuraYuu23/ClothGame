#include "MeshObject.h"
#include "../3D/Model/ModelManager.h"
#include "../3D/Model/ModelDraw.h"

void MeshObject::Initialize(LevelData::MeshData* data)
{

	serialNumber_ = newSerialNumber_;
	newSerialNumber_++;

	// 名前
	name_ = data->name;

	// クラス名前
	className_ = data->className;

	// ファイルの名前
	fileName_ = data->flieName;

	// ディレクトリパス
	directoryPath_ = data->directoryPath;

	// 親の名前
	parentName_ = data->parentName;

	// モデル
	model_ = ModelManager::GetInstance()->GetModel(directoryPath_, fileName_);

	// ワールドトランスフォーム
	worldTransform_.Initialize(model_->GetRootNode());
	worldTransform_.transform_ = data->transform;
	worldTransform_.UpdateMatrix();

	// マテリアル
	material_.reset(Material::Create());

	ColliderInitialize(data->collider);

	isDead_ = false;

}

void MeshObject::Update()
{

	prePosition_ = worldTransform_.GetWorldPosition();

}

void MeshObject::Draw(BaseCamera& camera)
{

	if (material_->GetMaterialMap()->color.w == 0.0f) {
		return;
	}

	ModelDraw::NormalObjectDesc desc;

	desc.model = model_;
	desc.material = material_.get();
	desc.camera = &camera;
	desc.worldTransform = &worldTransform_;

	ModelDraw::NormalObjectDraw(desc);

}

void MeshObject::CollisionListRegister(BaseCollisionManager* collisionManager)
{

	if (collider_) {
		collisionManager->ListRegister(collider_.get());
	}

}

void MeshObject::CollisionListRegister(BaseCollisionManager* collisionManager, ColliderDebugDraw* colliderDebugDraw)
{

	if (collider_) {
		collisionManager->ListRegister(collider_.get());

		colliderDebugDraw->AddCollider(*collider_.get());
	}

}

void MeshObject::ColliderInitialize(ColliderShape collider)
{

	// 値があるなら
	if (std::holds_alternative<OBB>(collider) || std::holds_alternative<Sphere>(collider)) {

		ColliderShape* colliderShape = new ColliderShape();

		*colliderShape = collider;

		collider_.reset(colliderShape);

	}

}

void MeshObject::SaveVelocityUpdate()
{

	saveVelocity_ = worldTransform_.GetWorldPosition() - prePosition_;

}
