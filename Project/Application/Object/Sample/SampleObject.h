#pragma once
#include "../../../Engine/3D/Model/Model.h"
#include "../../../Engine/Physics/RigidBody/RigidBody.h"
#include "../../../Engine/Animation/Animation.h"
#include "../../../Engine/PostEffect/Velocity2D/Velocity2DData.h"

class SampleObject
{

public:

	~SampleObject();

	void Initialize(Model* model);

	void Update();

	void Draw(BaseCamera camera);

	void ImGuiDraw();

	ID3D12Resource* GetVelocity2DData() { return velocity2DDataBuff_.Get(); }

private:

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// ローカル行列
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

	// 
	int32_t enableLighting_;

	float shininess_;

	//ノードアニメーション
	Animation animation_;

	RigidBody rigidBody_;

	// 速度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> velocity2DDataBuff_;

	Velocity2DData* velocity2DDataMap_;

	Vector3 velocity_;

};

