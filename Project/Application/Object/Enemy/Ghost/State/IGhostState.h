#pragma once
#include <cstdint>
#include "../../../../../Engine/Input/Input.h"
#include "../../../../../Engine/Math/Vector/Vector3.h"
#include "../../../../../Engine/3D/Transform/WorldTransform.h"

// 前方宣言
class Ghost;
class GhostStateSystem;

/// <summary>
/// プレイヤーの状態名
/// </summary>
enum GhostStateIndex {
	kGhostStateIndexRoot, // 通常
	kGhostStateIndexSideMove, // 横移動
	kGhostStateIndexBlownAway, // 吹き飛び
	kGhostStateIndexOfCount // 数
};

/// <summary>
/// ゴーストの状態の基盤
/// </summary>
class IGhostState
{

protected:

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
	/// ゴースト設定
	/// </summary>
	/// <param name="ghost"></param>
	void SetGhost(Ghost* ghost) { ghost_ = ghost; }

	/// <summary>
	/// ゴーストステートシステム設定
	/// </summary>
	/// <param name="ghostStateSystem"></param>
	void SetGhostStateSystem(GhostStateSystem* ghostStateSystem) { ghostStateSystem_ = ghostStateSystem; }

protected: // メンバ変数

	// ゴーストの状態番号
	uint32_t ghostStateNo_;

	// ゴースト
	Ghost* ghost_;

	// ゴーストステートシステム
	GhostStateSystem* ghostStateSystem_;

};

