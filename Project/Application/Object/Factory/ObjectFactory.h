#pragma once
#include "../../../Engine/Object/AbstractObjectFactory.h"
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/BaseObjectManager.h"

class ObjectFactory :
    public AbstractObjectFactory
{

public: // サブクラス,定数

	enum CreateObjectIndex {
		kCreateObjectIndexSkydome, // スカイドーム
		kCreateObjectIndexGround, // 地面
		kCreateObjectIndexGroundBlock, // 地面ブロック版
		kCreateObjectIndexPlayer, // プレイヤー
		kCreateObjectIndexClothGate, // 布ゲート
		kCreateObjectIndexGhost, // ゴースト
		kCreateObjectIndexOfCount, // 数数える用
	};

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
	/// <param name="currentGenerationCount">生成回数</param>
	/// <returns>オブジェクト</returns>
	IObject* CreateObjectPattern(LevelData::ObjectData& objectData, uint32_t currentGenerationCount);

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

