#pragma once
#include "../../Math/Matrix/Matrix4x4.h"
#include <string>
#include <vector>
#include "../Transform/TransformStructure.h"

/// <summary>
/// モデルのノード
/// </summary>
class ModelNode
{

public: //メンバ関数

	Matrix4x4 localMatrix; // ローカル行列
	Matrix4x4 offsetMatrix; // オフセット行列
	std::string name; // 名前
	std::vector<ModelNode> children; // 子供
	uint32_t meshNum; // メッシュ番号
	QuaternionTransform initTransform; // 初期トランスフォーム

};

