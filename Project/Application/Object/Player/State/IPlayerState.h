#pragma once
#include <cstdint>
#include "../../../../Engine/Input/Input.h"
#include "../../../../Engine/Math/Vector/Vector3.h"
#include "../../../../Engine/3D/Transform/WorldTransform.h"

class Player;

//プレイヤーの状態名
enum PlayerStateIndex {
	kPlayerStateIndexRoot, // 通常
	kPlayerStateIndexOfCount // 数
};

class IPlayerState
{

protected:

	// プレイヤーの状態番号
	static uint32_t playerStateNo_;

	// プレイヤーのモーション番号
	static uint32_t playerMotionNo_;

	// プレイヤー
	static Player* player_;

	//インスタンス
	static Input* input_;

	// 自動移動速度
	static const float kAutoMoveSpeed_;

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IPlayerState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	// プレイヤーの状態番号
	uint32_t GetPlaryerStateNo() { return playerStateNo_; }

	// プレイヤーのモーション番号
	uint32_t GetPlaryerMotionNo() { return playerMotionNo_; }

	// プレイヤーセット
	void SetPlayer(Player* player) { player_ = player; }

protected: // メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ワープ
	/// </summary>
	void Warp();

protected: // メンバ変数

	// 移動速度
	Vector3 moveVelocity_;

	// ワールドトランスフォーム
	WorldTransform* playerWorldTransform_;

};

