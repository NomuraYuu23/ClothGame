#pragma once
#include "AnimationData.h"
#include "../3D/Transform/TransformStructure.h"

class Animation
{

public:

	/// <summary>
	/// 計算データ
	/// </summary>
	struct AnimationCalcData {

		AnimationData animation; // アニメーションデータ
		bool isRun; // 実行しているか
		double timer; // タイマー

		bool isLoop; // ループか
		bool isFinished; // 終了したか

	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="AnimationData">アニメーションデータ</param>
	/// <param name="initTransforms">初期トランスフォーム(ノード数)</param>
	/// <param name="nodeNames">ノード名前</param>
	void Initialize(
		const std::vector<AnimationData>& animationDatas,
		const std::vector<QuaternionTransform>& initTransforms,
		const std::vector<std::string>& nodeNames);

	/// <summary>
	/// アニメーション
	/// </summary>
	/// <returns>ローカル行列(ノード数)</returns>
	std::vector<Matrix4x4> AnimationUpdate();
	
	/// <summary>
	/// アニメーションを開始させる
	/// </summary>
	/// <param name="animationNum">アニメーション番号</param>
	/// <param name="isLoop">ループさせるか</param>
	void StartAnimation(uint32_t animationNum, bool isLoop);

	/// <summary>
	/// アニメーションを停止させる(リセット)
	/// </summary>
	/// <param name="animationNum">アニメーション番号</param>
	void StopAnimation(uint32_t animationNum);

	/// <summary>
	/// アニメーションが終了したか
	/// </summary>
	/// <returns>アニメーション終了したか(ノード数)</returns>
	std::vector<bool> FinishedAnimations();

	/// <summary>
	/// 移動補間係数セット
	/// </summary>
	/// <param name="moveT">移動補間係数</param>
	void SetMoveT(float moveT) { moveT_ = moveT; }

	/// <summary>
	/// アニメーションの実行状態取得
	/// </summary>
	/// <returns>アニメーションの実行状態</returns>
	std::vector<bool> GetRunningAnimations();

	/// <summary>
	/// 計算データ取得
	/// </summary>
	/// <returns></returns>
	std::vector<AnimationCalcData>* GetAnimationDatas() { return &animationDatas_; }

	/// <summary>
	/// アニメーションタイマーを固定する
	/// </summary>
	/// <param name="animationNum">アニメーション番号</param>
	/// <param name="time">タイム</param>
	void AnimationTimerFix(uint32_t animationNum, double time);

private:

	/// <summary>
	/// ノードアニメーション更新
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <param name="timer">時間</param>
	void NodeAnimationUpdate(uint32_t index, double timer);

private:

	// 計算データ
	std::vector<AnimationCalcData> animationDatas_;
	// アニメーション数
	uint32_t animationCalcDataNum_ = 0;

	// ノード数
	uint32_t nodeNum_ = 0;

	// トランスフォーム
	std::vector<QuaternionTransform> transforms_;

	// アニメーション速度
	double animationSpeed_ = 0.0;

	// ノードの名前
	std::vector<std::string> nodeNames_;

	// 目指す位置 初期行列と同じ分だけ
	//std::vector<Vector3> targetPositions_;
	std::vector<uint32_t> positionAddCount_;
	// 目指す回転 初期行列と同じ分だけ
	//std::vector<Quaternion> targetRotations_;
	std::vector<uint32_t> rotationAddCount_;
	// 目指す大きさ 初期行列と同じ分だけ
	//std::vector<Vector3> targetScalings_;
	std::vector<uint32_t> scalingAddCount_;

	// 目指すトランスフォーム
	std::vector<QuaternionTransform> targetTransforms_;

	// 移動補間係数
	float moveT_ = 0.2f;

};
