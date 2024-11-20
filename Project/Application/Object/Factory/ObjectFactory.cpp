#include "ObjectFactory.h"
#include "../../../Engine/Object/MeshObject.h"
#include "ObjectCreate.h"

#include "../ClothGate/ClothGate.h"
#include "../Enemy/Ghost/Ghost.h"

// オブジェクト作成でそれぞれのタイプを作成するための関数群
// 返り値 無し
// 引数1 オブジェクト
std::array<
	std::pair<std::string, std::function<IObject*(LevelData::ObjectData&)>>,
	ObjectFactory::CreateObjectIndex::kCreateObjectIndexOfCount> ObjectFactory::createObjectFunctions_ = {};

// マネージャー
BaseObjectManager* ObjectFactory::objectManager_ = nullptr;

void ObjectFactory::Initialize(BaseObjectManager* objectManager)
{

	objectManager_ = objectManager;

	// 関数を入れていく

	// スカイドーム
	createObjectFunctions_[kCreateObjectIndexSkydome].first = "Skydome";
	createObjectFunctions_[kCreateObjectIndexSkydome].second = ObjectCreate::CreateObjectSkydome;

	// 地面
	createObjectFunctions_[kCreateObjectIndexGround].first = "Ground";
	createObjectFunctions_[kCreateObjectIndexGround].second = ObjectCreate::CreateObjectGround;

	// プレイヤー
	createObjectFunctions_[kCreateObjectIndexPlayer].first = "Player";
	createObjectFunctions_[kCreateObjectIndexPlayer].second = ObjectCreate::CreateObjectPlayer;

	// 布ゲート
	createObjectFunctions_[kCreateObjectIndexClothGate].first = "ClothGate";
	createObjectFunctions_[kCreateObjectIndexClothGate].second = ObjectCreate::CreateObjectClothGate;

	// 布ゲート
	createObjectFunctions_[kCreateObjectIndexGhost].first = "Ghost";
	createObjectFunctions_[kCreateObjectIndexGhost].second = ObjectCreate::CreateObjectGhost;

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
			}

		}

	}

    return object;

}

IObject* ObjectFactory::CreateObjectPattern(LevelData::ObjectData& objectData, uint32_t currentGenerationCount)
{

	IObject* object = nullptr;

	// パターンの始まる距離
	const float kPatternStartPosition = -400.0f;
	// パターン区間の大きさ
	const float kPatternSize = 100.0f;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
				// パターン回数に応じて移動
				object->GetWorldTransformAdress()->transform_.translate.z += currentGenerationCount * kPatternSize + kPatternStartPosition;
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
