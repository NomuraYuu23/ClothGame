#pragma once
#include <string>

#include "IObject.h"
#include "../Level/LevelData.h"

class AbstractObjectFactory {

public: //メンバ関数

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AbstractObjectFactory() = default;

	/// <summary>
	/// オブジェクト作成
	/// </summary>
	/// <param name="objectData">オブジェクトデータ/param>
	/// <returns>オブジェクト</returns>
	virtual IObject* CreateObject(LevelData::ObjectData& objectData) = 0;

};
