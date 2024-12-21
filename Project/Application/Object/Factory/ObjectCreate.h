#pragma once
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/IObject.h"

// 前方宣言
class Player;

/// <summary>
/// オブジェクト作成関数をまとめたもの
/// </summary>
class ObjectCreate
{

public: //メンバ関数

	/// <summary>
	/// 地面ブロック
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGroundBlock(LevelData::ObjectData& objectData);

	/// <summary>
	/// スカイドーム
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectSkydome(LevelData::ObjectData& objectData);

	/// <summary>
	/// プレイヤー
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectPlayer(LevelData::ObjectData& objectData);

	/// <summary>
	/// 布ゲート
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectClothGate(LevelData::ObjectData& objectData);

	/// <summary>
	/// 大きい布ゲート
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectBigClothGate(LevelData::ObjectData& objectData);

	/// <summary>
	/// ゴースト
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGhost(LevelData::ObjectData& objectData);

	/// <summary>
	/// 横に置く布
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectSideCloth(LevelData::ObjectData& objectData);

private: // メンバ変数

	// プレイヤー
	static Player* player_;

};

