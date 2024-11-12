#pragma once
#include <cstdint>
#include "../../../../Engine/Animation/Animation.h"
#include "../../../../Engine/Animation/LocalMatrixManager.h"
#include "../../../../Engine/3D/Model/Model.h"

class PlayerAnimation
{

public:

	// モーション番号
	enum PlayerMotionIndex {
		kPlayerMotionIndexWait, // 待ち
		kPlayerMotionIndexOfCount // 数
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル/param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="stateMotionNo">ステートのモーション番号</param>
	void Update(uint32_t stateMotionNo);

public: // アクセッサ

	/// <summary>
	/// ローカル行列取得
	/// </summary>
	/// <returns></returns>
	LocalMatrixManager* GetLocalMatrixManager() { return localMatrixManager_.get();	}

private: // パーツ,アニメーション変数

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	//ノードアニメーション
	Animation animation_;

	// ローカル行列マネージャ
	std::unique_ptr<LocalMatrixManager> localMatrixManager_ = nullptr;

};

