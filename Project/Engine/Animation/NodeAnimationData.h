#pragma once
#include <vector>
#include "../Math/Vector/Vector3.h"
#include "../Math/Matrix/Matrix4x4.h"
#include "../Math/Quaternion.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// アニメーションキーVector3
/// </summary>
struct AnimationVector3Key
{

	// タイム 秒
	double time;
	// 値
	Vector3 value;

};

/// <summary>
/// アニメーションキーQuaternion
/// </summary>
struct AnimationQuaternionKey
{

	// タイム 秒
	double time;
	// 値
	Quaternion value;

};

/// <summary>
/// ノードアニメーションデータ
/// </summary>
class NodeAnimationData
{

public:

	// 秒に変換するための値
	static const double kSecondsConversion_;

public: // 読み込み

	// 初期化
	void Initialize(const aiNodeAnim& nodeAnim);

public: // 読み込みデータ

	// 位置
	std::vector<AnimationVector3Key> positions_;
	// 位置の数
	uint32_t positionKeyNum_;
	// 回転
	std::vector<AnimationQuaternionKey> rotations_;
	// 回転の数
	uint32_t rotationKeyNum_;
	// 大きさ
	std::vector<AnimationVector3Key> scalings_;
	// 大きさの数
	uint32_t scalingKeyNum_;

	// 終了時間 秒
	double endTime_;

	// ノード名前
	std::string nodeName_;

};

