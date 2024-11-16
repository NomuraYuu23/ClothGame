#include "BaseObjectManager.h"
#include "MeshObject.h"

void BaseObjectManager::Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager)
{

	// レベルデータマネージャー
	levelDataManager_ = levelDataManager;

	// コライダーのデバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	colliderDebugDraw_->Initialize();

	// リセット
	Reset(levelIndex);

	//オブジェクトファクトリーを取得しているか
	assert(objectFactory_);

}

void BaseObjectManager::Update()
{

#ifdef _DEMO

	// リストをクリア
	colliderDebugDraw_->ListClear();

#endif // _DEMO

	// フラグによる死亡処理
	objects_.remove_if([this](ObjectPair& objects) {
		if (objects.second->GetIsDead()) {
			objects.second.reset();
			return true;
		}
		return false;
		});

	// 更新処理
	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		it->second->Update();
	}

}

void BaseObjectManager::Draw(BaseCamera& camera)
{

	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<MeshObject*>(it->second.get())->Draw(camera);
	}

}

void BaseObjectManager::Draw(BaseCamera& camera, DrawLine* drawLine)
{

	Draw(camera);

	// コライダーのデバッグ描画
	colliderDebugDraw_->DrawMap(drawLine);

}

void BaseObjectManager::ParticleDraw(BaseCamera& camera)
{

	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<MeshObject*>(it->second.get())->ParticleDraw(camera);
	}

}

void BaseObjectManager::ImGuiDraw()
{

#ifdef _DEMO

	colliderDebugDraw_->ImGuiDraw();

	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		it->second->ImGuiDraw();
	}

#endif // _DEMO


}

IObject* BaseObjectManager::GetObjectPointer(const std::string name)
{

	IObject* result = nullptr;

	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {

		if (it->first == name) {
			result = it->second.get();
			return result;
		}

	}

	return nullptr;

}

void BaseObjectManager::CollisionListRegister(BaseCollisionManager* collisionManager)
{

	bool isDebug = false;

#ifdef _DEMO
	isDebug = true;
#endif // _DEBUG

	for (std::list<ObjectPair>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {

		if (isDebug) {
			static_cast<MeshObject*>(it->second.get())->CollisionListRegister(collisionManager, colliderDebugDraw_.get());
		}
		else {
			static_cast<MeshObject*>(it->second.get())->CollisionListRegister(collisionManager);
		}

	}

}

void BaseObjectManager::Reset(LevelIndex levelIndex)
{

	// レベルデータの取得
	LevelData* levelData = levelDataManager_->GetLevelDatas(levelIndex);

	// レベルデータのオブジェクトを走査
	for (std::vector<LevelData::ObjectData>::iterator it = levelData->objectsData_.begin();
		it != levelData->objectsData_.end(); ++it) {

		// オブジェクトの参照
		LevelData::ObjectData objectData = *it;

		// 型にあわせてInitialize
		std::unique_ptr<IObject> object;
		object.reset(objectFactory_->CreateObject(objectData));

		if (object) {

			// listへ
			objects_.emplace_back(object->GetName(), std::move(object));
		}

	}

}

IObject* BaseObjectManager::AddObject(LevelData::ObjectData& data)
{

	// 型にあわせてInitialize
	std::unique_ptr<IObject> object;
	object.reset(objectFactory_->CreateObject(data));

	if (object) {

		// listへ
		objects_.emplace_back(object->GetName(), std::move(object));
	}

	return objects_.back().second.get();

}

