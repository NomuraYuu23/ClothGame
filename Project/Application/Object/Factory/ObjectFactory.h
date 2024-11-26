#pragma once
#include "../../../Engine/Object/AbstractObjectFactory.h"
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/BaseObjectManager.h"
#include "CreateObjectIndex.h"

class ObjectFactory :
    public AbstractObjectFactory
{

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	/// <param name="bossSystem">ボスシステム</param>
	void Initialize(BaseObjectManager* objectManager);

	/// <summary>
	/// オブジェクト作成
	/// </summary>
	/// <param name="objectData">オブジェクトデータ/param>
	/// <returns>オブジェクト</returns>
	IObject* CreateObject(LevelData::ObjectData& objectData) override;

	/// <summary>
	/// オブジェクト作成Patternバージョン
	/// </summary>
	/// <param name="objectData">オブジェクトデータ/param>
	/// <returns>オブジェクト</returns>
	IObject* CreateObjectPattern(LevelData::ObjectData& objectData);

	/// <summary>
	/// オブジェクトマネージャー設定
	/// </summary>
	/// <param name="objectManager"></param>
	void SetObjectManager(BaseObjectManager* objectManager) { objectManager_ = objectManager; }

private:

	// オブジェクト作成でそれぞれのタイプを作成するための関数群
	// 返り値 無し
	// 引数1 オブジェクト
	static std::array<
		std::pair<std::string,std::function<IObject* (LevelData::ObjectData&)>>,
		CreateObjectIndex::kCreateObjectIndexOfCount> createObjectFunctions_;

	// マネージャー
	static BaseObjectManager* objectManager_;

};

