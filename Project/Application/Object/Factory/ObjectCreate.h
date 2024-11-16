#pragma once
#include "../../../Engine/Level/LevelData.h"
#include "../../../Engine/Object/IObject.h"

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

};

