#pragma once
#include "../../../Engine/Object/MeshObject.h"

class Ground :
    public MeshObject
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

};

