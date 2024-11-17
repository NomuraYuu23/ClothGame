#pragma once
#include "../../../Engine/Object/MeshObject.h"

class Skydome : 
	public MeshObject 
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">データ</param>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数

	// 回転速度
	const float kRotateSpeed_ = 0.001f;

};