#pragma once
#include "../../../Engine/Object/MeshObject.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

// 前方宣言
class Player;

class BaseClothGate :
    public MeshObject
{

protected: 

    // プレイヤーの衝突判定データの名前
    static const std::string kPlayerColliderName_;

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
	/// 布リセット
	/// </summary>
	virtual void ClothReset() = 0;

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

public: // アクセッサ

	/// <summary>
	/// プレイヤー設定
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

protected:

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

};
