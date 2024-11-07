#pragma once
#include "../../Engine/3D/Model/Model.h"
#include "../../Engine/3D/Material/Material.h"
#include "../../Engine/3D/Transform/WorldTransform.h"

class Skydome {

public:

	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(BaseCamera& camera);
	
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> material_ = nullptr;
	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

	// 回転速度
	float rotateSpeed_ = 0.001f;

};