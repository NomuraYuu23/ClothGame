#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

// 前方宣言
class Player;

class ClothGate :
    public MeshObject
{

public: // 静的メンバ変数

	// プレイヤーの衝突判定データの名前
	static const std::string kPlayerColliderName_;
	// DirectX
	static DirectXCommon* dxCommon_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera& camera) override;
	
public: // アクセッサ

	/// <summary>
	/// プレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// 布
	std::unique_ptr<ClothGPU> cloth_;

	// プレイヤーの衝突判定データ
	ClothGPUCollision::Sphere playerCollider_;

	// プレイヤー
	Player* player_;

};

