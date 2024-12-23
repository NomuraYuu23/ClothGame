#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../GPUParticle/PassThroughClothParticle/PassThroughClothParticle.h"

// 前方宣言
class Player;

/// <summary>
/// 布ゲートの基盤
/// </summary>
class BaseClothGate :
    public MeshObject
{

protected: 

    // プレイヤーの衝突判定データの名前
    static const std::string kPlayerColliderName_;

	// パーティクルエミッタの追加位置
	static const Vector3 kParticleEmitAddPosition_;

	// DirectX
	static DirectXCommon* dxCommon_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="data">生成データ</param>
	virtual void Initialize(LevelData::MeshData* data) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void Draw(BaseCamera& camera) override;

	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void ParticleDraw(BaseCamera& camera) override;

	/// <summary>
	/// 布リセット
	/// </summary>
	virtual void ClothReset() = 0;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	virtual void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

protected: // メンバ関数

	/// <summary>
	/// 布初期化
	/// </summary>
	/// <param name="clothScale">大きさ</param>
	/// <param name="clothDiv">分割数</param>
	virtual void ClothInitialize(const Vector2& clothScale, const Vector2& clothDiv);

	/// <summary>
	/// 布更新
	/// </summary>
	virtual void ClothUpdate();

	/// <summary>
	/// コライダー更新
	/// </summary>
	virtual void ColliderUpdate();

public: // アクセッサ

	/// <summary>
	/// プレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

protected: // メンバ変数

	// 布
	std::unique_ptr<ClothGPU> cloth_;

	// プレイヤーの衝突判定データ
	ClothGPUCollision::Capsule playerCollider_;

	// プレイヤー
	Player* player_;

	// プレイヤーを登録しているか
	bool registeringPlayer_;

	// 更新秒数
	float updateSeconds_;

	// コライダーの位置
	Vector3 colliderAddPosition_;

	// くぐられた時のパーティクル
	std::unique_ptr<PassThroughClothParticle> passThroughClothParticle_;

	// くぐられたか
	bool passedThrough_;

};

