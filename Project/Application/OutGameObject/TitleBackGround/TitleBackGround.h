#pragma once
#include "../../../Engine/3D/Model/Model.h"

/// <summary>
/// タイトルの背景
/// </summary>
class TitleBackGround
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);

private:

	// モデル
	Model* model_ = nullptr;

	// マテリアル
	std::unique_ptr<Material> material_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

};

