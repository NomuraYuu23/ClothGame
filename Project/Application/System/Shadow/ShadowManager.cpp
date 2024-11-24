#include "ShadowManager.h"
#include "Shadow.h"

void ShadowManager::Initialize(Model* model)
{

	LargeNumberOfObjects::Initialize(model);


	// 影を出す数
	shadowCount_ = 0u;

	// 影が最大か
	isShadowMax_ = false;

	// 影の位置追加分
	posYAdd_ = 0.1f;

}

void ShadowManager::Update()
{

	// 影候補が衝突しているか
	SeeShadow();

}

void ShadowManager::PreUpdate()
{

	// オブジェクトを削除
	objects_.remove_if([](std::unique_ptr<OneOfManyObjects>& object) {
		object.reset();
		return true;
		});

	// リストをクリア
	castsShadowObjList_.clear();
	shadowAppearsObjList_.clear();

}

void ShadowManager::CastsShadowObjListRegister(MeshObject* object)
{

	OBB obb = std::get<OBB>(*object->GetCollider());

	ShadowCandidate shadow = { obb.center_ , obb.size_ };

	castsShadowObjList_.push_back(shadow);

}

void ShadowManager::ShadowAppearsObjListRegister(MeshObject* object)
{

	OBB obb = std::get<OBB>(*object->GetCollider());

	ShadowCandidate shadow = { obb.center_ , obb.size_ };

	shadowAppearsObjList_.push_back(shadow);

}

void ShadowManager::SeeShadow()
{

	shadowCount_ = 0u;
	isShadowMax_ = false;

	// 影を発生させるオブジェクトリスト
	std::list<ShadowCandidate>::iterator itrCastsShadowObj = castsShadowObjList_.begin();
	for (; itrCastsShadowObj != castsShadowObjList_.end(); ++itrCastsShadowObj) {

		// 影が現れるオブジェクトリスト	
		std::list<ShadowCandidate>::iterator itrShadowAppearsObj = shadowAppearsObjList_.begin();
		for (; itrShadowAppearsObj != shadowAppearsObjList_.end(); ++itrShadowAppearsObj) {

			// 影が出るか確認
			if (CollisionCheck(*itrCastsShadowObj, *itrShadowAppearsObj)) {
				//break;
			}

		}

		// 影が最大
		if (isShadowMax_) {
			return;
		}

	}

}

bool ShadowManager::CollisionCheck(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears)
{

	// 高さ確認
	if (castsShadow.position_.y + castsShadow.size_.y >= shadowAppears.position_.y + shadowAppears.size_.y) {

		// 完全に内包している
		if (castsShadow.position_.x - castsShadow.size_.x >= shadowAppears.position_.x - shadowAppears.size_.x &&
			castsShadow.position_.x + castsShadow.size_.x <= shadowAppears.position_.x + shadowAppears.size_.x &&
			castsShadow.position_.z - castsShadow.size_.z >= shadowAppears.position_.z - shadowAppears.size_.z &&
			castsShadow.position_.z + castsShadow.size_.z <= shadowAppears.position_.z + shadowAppears.size_.z) {
			CompriseOnCollision(castsShadow, shadowAppears);
		}
		// 完全に内包していないが衝突
		else if (
			castsShadow.position_.x + castsShadow.size_.x >= shadowAppears.position_.x - shadowAppears.size_.x &&
			castsShadow.position_.x - castsShadow.size_.x <= shadowAppears.position_.x + shadowAppears.size_.x &&
			castsShadow.position_.z + castsShadow.size_.z >= shadowAppears.position_.z - shadowAppears.size_.z &&
			castsShadow.position_.z - castsShadow.size_.z <= shadowAppears.position_.z + shadowAppears.size_.z) {
			NotCompriseOnCollision(castsShadow, shadowAppears);
		}
		// 衝突していない
		else {
			return false;
		}

		// 影の数制限
		ShadowLimit();
		return true;
	
	}

	return false;

}

void ShadowManager::CompriseOnCollision(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears)
{

	// 影作成
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Shadow>();
	obj->Initialize();
	
	// ワールドトランスフォーム
	obj->transform_.translate.x = castsShadow.position_.x;
	obj->transform_.translate.y = shadowAppears.position_.y + shadowAppears.size_.y + posYAdd_;
	obj->transform_.translate.z = castsShadow.position_.z;
	obj->transform_.scale = castsShadow.size_;
	obj->transform_.scale.y = 1.0f;
	obj->Update();

	// マテリアル
	Matrix4x4 uvTransform = 
		Matrix4x4::MakeAffineMatrix(
			Vector3{ 0.33f, 0.33f, 1.0f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.33f, 0.33f, 0.0f });

	obj->materialData_.uvTransform = uvTransform;

	// 追加
	objects_.push_back(std::move(obj));

}

void ShadowManager::NotCompriseOnCollision(const ShadowCandidate& castsShadow, const ShadowCandidate& shadowAppears)
{

	// 影作成
	std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Shadow>();
	obj->Initialize();

	// ワールドトランスフォーム
	obj->transform_.translate.x = castsShadow.position_.x;
	obj->transform_.translate.y = shadowAppears.position_.y + shadowAppears.size_.y + posYAdd_;
	obj->transform_.translate.z = castsShadow.position_.z;
	obj->transform_.scale = castsShadow.size_;
	obj->transform_.scale.y = 1.0f;

	// マテリアル
	Vector3 materialPosition = { 0.33f, 0.33f, 0.0f };
	float move = 0.0f;

	// x
	if (castsShadow.position_.x + castsShadow.size_.x > shadowAppears.position_.x - shadowAppears.size_.x &&
		castsShadow.position_.x - castsShadow.size_.x < shadowAppears.position_.x - shadowAppears.size_.x) {
		obj->transform_.translate.x = shadowAppears.position_.x - shadowAppears.size_.x + castsShadow.size_.x;
		move = 0.33f * (std::fabsf((shadowAppears.position_.x - shadowAppears.size_.x) - (castsShadow.position_.x - castsShadow.size_.x)) / castsShadow.size_.x / 2.0f);
		materialPosition.x = 0.33f - move;
	}
	else if (castsShadow.position_.x - castsShadow.size_.x < shadowAppears.position_.x + shadowAppears.size_.x &&
		castsShadow.position_.x + castsShadow.size_.x > shadowAppears.position_.x + shadowAppears.size_.x) {
		obj->transform_.translate.x = shadowAppears.position_.x + shadowAppears.size_.x - castsShadow.size_.x;
		move = 0.33f * (std::fabsf((shadowAppears.position_.x + shadowAppears.size_.x) - (castsShadow.position_.x + castsShadow.size_.x)) / castsShadow.size_.x / 2.0f);
		materialPosition.x = 0.33f + move;
	}

	// z
	if (castsShadow.position_.z + castsShadow.size_.z > shadowAppears.position_.z - shadowAppears.size_.z &&
		castsShadow.position_.z - castsShadow.size_.z < shadowAppears.position_.z - shadowAppears.size_.z) {
		obj->transform_.translate.z = shadowAppears.position_.z - shadowAppears.size_.z + castsShadow.size_.z;
		move = 0.33f * (std::fabsf((shadowAppears.position_.z - shadowAppears.size_.z) - (castsShadow.position_.z - castsShadow.size_.z)) / castsShadow.size_.z / 2.0f);
		materialPosition.y = 0.33f + move;
	}
	else if (castsShadow.position_.z - castsShadow.size_.z < shadowAppears.position_.z + shadowAppears.size_.z &&
		castsShadow.position_.z + castsShadow.size_.z > shadowAppears.position_.z + shadowAppears.size_.z) {
		obj->transform_.translate.z = shadowAppears.position_.z + shadowAppears.size_.z - castsShadow.size_.z;
		move = 0.33f * (std::fabsf((shadowAppears.position_.z + shadowAppears.size_.z) - (castsShadow.position_.z + castsShadow.size_.z)) / castsShadow.size_.z / 2.0f);
		materialPosition.y = 0.33f - move;
	}

	Matrix4x4 uvTransform =
		Matrix4x4::MakeAffineMatrix(
			Vector3{ 0.33f, 0.33f, 1.0f }, Vector3{ 0.0f,0.0f,0.0f }, materialPosition);

	obj->materialData_.uvTransform = uvTransform;

	// ワールドトランスフォーム更新
	obj->Update();

	// 追加
	objects_.push_back(std::move(obj));

}

void ShadowManager::ShadowLimit()
{

	// 影の数を増やす
	shadowCount_++;

	// 影の作成最大値
	if (shadowCount_ == kNumInstanceMax_) {
		isShadowMax_ = true;
	}

}
