#pragma once

#include "IObject.h"
#include "../Camera/BaseCamera.h"

#include <list>
#include "../Level/LevelIndex.h"
#include "../Level/LevelDataManager.h"
#include "../Collider/ColliderDebugDraw/ColliderDebugDraw.h"
#include "../Collision/BaseCollisionManager.h"
#include "AbstractObjectFactory.h"

class BaseObjectManager
{

public: //virtual

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	/// <param name="levelDataManager">レベルデータマネージャー</param>
	virtual void Initialize(LevelIndex levelIndex, LevelDataManager* levelDataManager);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(BaseCamera& camera);

	/// <summary>
	/// 描画(デバッグ)
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="drawLine">線描画クラス</param>
	virtual void Draw(BaseCamera& camera, DrawLine * drawLine);

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void ParticleDraw(BaseCamera& camera);

	/// <summary>
	///	ImGui描画
	/// </summary>
	virtual void ImGuiDraw();

public: //virtualではない

	/// <summary>
	/// オブジェクト検索
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	IObject* GetObjectPointer(const std::string name);

	/// <summary>
	/// コライダー登録
	/// </summary>
	/// <param name="collisionManager"></param>
	void CollisionListRegister(BaseCollisionManager* collisionManager);

	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="levelIndex">レベル番号</param>
	void Reset(LevelIndex levelIndex);

	/// <summary>
	/// オブジェクト手動追加
	/// </summary>
	/// <param name="data">オブジェクトデータ</param>
	/// <returns>オブジェクトのポインタ</returns>
	IObject* AddObject(LevelData::ObjectData& data);

protected:

	using ObjectPair = std::pair<std::string, std::unique_ptr<IObject>>;

	// オブジェクト
	std::list<ObjectPair> objects_{};

	// コライダーのデバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_;

	// レベルデータマネージャー
	LevelDataManager* levelDataManager_;

	// オブジェクトファクトリー
	std::unique_ptr<AbstractObjectFactory> objectFactory_;

};

