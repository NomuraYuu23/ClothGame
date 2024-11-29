#include "ObjectFactory.h"
#include "../../../Engine/Object/MeshObject.h"
#include "ObjectCreate.h"

#include "../ClothGate/ClothGate.h"
#include "../SideCloth/SideCloth.h"
#include "../Enemy/Ghost/Ghost.h"
#include "CreateObjectNames.h"

// オブジェクト作成でそれぞれのタイプを作成するための関数群
// 返り値 無し
// 引数1 オブジェクト
std::array<
	std::pair<std::string, std::function<IObject*(LevelData::ObjectData&)>>,
	CreateObjectIndex::kCreateObjectIndexOfCount> ObjectFactory::createObjectFunctions_ = {};

// マネージャー
BaseObjectManager* ObjectFactory::objectManager_ = nullptr;

void ObjectFactory::Initialize(BaseObjectManager* objectManager)
{

	objectManager_ = objectManager;

	// 関数を入れていく

	// スカイドーム
	createObjectFunctions_[kCreateObjectIndexSkydome].first = kCreateObjectNames_[kCreateObjectIndexSkydome];
	createObjectFunctions_[kCreateObjectIndexSkydome].second = ObjectCreate::CreateObjectSkydome;

	// 地面
	createObjectFunctions_[kCreateObjectIndexGroundBlock].first = kCreateObjectNames_[kCreateObjectIndexGroundBlock];
	createObjectFunctions_[kCreateObjectIndexGroundBlock].second = ObjectCreate::CreateObjectGroundBlock;

	// プレイヤー
	createObjectFunctions_[kCreateObjectIndexPlayer].first = kCreateObjectNames_[kCreateObjectIndexPlayer];
	createObjectFunctions_[kCreateObjectIndexPlayer].second = ObjectCreate::CreateObjectPlayer;

	// 布ゲート
	createObjectFunctions_[kCreateObjectIndexClothGate].first = kCreateObjectNames_[kCreateObjectIndexClothGate];
	createObjectFunctions_[kCreateObjectIndexClothGate].second = ObjectCreate::CreateObjectClothGate;

	// ゴースト
	createObjectFunctions_[kCreateObjectIndexGhost].first = kCreateObjectNames_[kCreateObjectIndexGhost];
	createObjectFunctions_[kCreateObjectIndexGhost].second = ObjectCreate::CreateObjectGhost;

	// 横に置く布
	createObjectFunctions_[kCreateObjectIndexSideCloth].first = kCreateObjectNames_[kCreateObjectIndexSideCloth];
	createObjectFunctions_[kCreateObjectIndexSideCloth].second = ObjectCreate::CreateObjectSideCloth;

}

IObject* ObjectFactory::CreateObject(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
				if (i == kCreateObjectIndexSideCloth) {
					static_cast<SideCloth*>(object)->ClothReset();
				}
			}

		}

	}

    return object;

}

IObject* ObjectFactory::CreateObjectPattern(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// パターンの始まる距離
	const float kPatternStartPosition = -400.0f;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
				// 移動
				object->GetWorldTransformAdress()->transform_.translate.z += kPatternStartPosition;
				object->GetWorldTransformAdress()->UpdateMatrix();
				if (i == kCreateObjectIndexClothGate) {
					static_cast<ClothGate*>(object)->ClothReset();
				}
				else if (i == kCreateObjectIndexGhost) {
					static_cast<Ghost*>(object)->ClothReset();
				}

			}

		}

	}

	return object;

}
