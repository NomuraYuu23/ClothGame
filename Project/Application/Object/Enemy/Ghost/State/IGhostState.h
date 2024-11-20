#pragma once
#include <cstdint>
#include "../../../../../Engine/Input/Input.h"
#include "../../../../../Engine/Math/Vector/Vector3.h"
#include "../../../../../Engine/3D/Transform/WorldTransform.h"

class Ghost;
class GhostStateSystem;

//プレイヤーの状態名
enum GhostStateIndex {
	kGhostStateIndexRoot, // 通常
	kGhostStateIndexOfCount // 数
};

class IGhostState
{

protected:

	// ゴーストの状態番号
	static uint32_t ghostStateNo_;

	// ゴーストのモーション番号
	static uint32_t ghostMotionNo_;

	// ゴースト
	static Ghost* ghost_;

	// ゴーストステートシステム
	static GhostStateSystem* ghostStateSystem_;

	// 入力
	static Input* input_;

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IGhostState() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

public: // アクセッサ

	/// <summary>
	/// ゴーストの状態番号取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetGhostStateNo() { return ghostStateNo_; }

	/// <summary>
	/// ゴーストのモーション番号取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetGhostMotionNo() { return ghostMotionNo_; }

	/// <summary>
	/// ゴースト設定
	/// </summary>
	/// <param name="ghost"></param>
	void SetGhost(Ghost* ghost) { ghost_ = ghost; }

	/// <summary>
	/// ゴーストステートシステム設定
	/// </summary>
	/// <param name="ghostStateSystem"></param>
	void SetGhostStateSystem(GhostStateSystem* ghostStateSystem) { ghostStateSystem_ = ghostStateSystem; }

};

