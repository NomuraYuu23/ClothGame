#pragma once
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/IObject.h"

class Player;

class ObjectCreate
{

public: //メンバ関数

	/// <summary>
	/// 地面
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGround(LevelData::ObjectData& objectData);

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
	/// ゴースト
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGhost(LevelData::ObjectData& objectData);

private: // メンバ変数

	// プレイヤー
	static Player* player_;

};

